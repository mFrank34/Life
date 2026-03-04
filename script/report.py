#!/usr/bin/env python3
"""
Profile Report Generator
Author: Michael Franks
Description: Generates a PDF report with graphs and summary from profile JSON data.

Folder structure expected:
  output/
    c32_r8/raw/profile_cache_c32_r8.json
    c32_r8/raw/profile_sparse_c32_r8.json
    ...

Usage:
  ./generate_report.py output/          # process entire output folder
  ./generate_report.py profile.json     # single file
"""

import glob
import io
import json
import os
import sys

import matplotlib
import numpy as np

matplotlib.use("Agg")
import matplotlib.pyplot as plt

from pathlib import Path
from reportlab.lib.pagesizes import A4
from reportlab.lib import colors
from reportlab.lib.units import cm
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, Image,
    Table, TableStyle, PageBreak, HRFlowable
)
from reportlab.lib.enums import TA_CENTER

# --- Colours ---

COL_UPDATE = "#4a90d9"
COL_RENDER = "#e8a838"
COL_TOTAL = "#e85454"
COL_CHUNKS = "#7ed17e"
COL_MEMORY = "#b07adb"
COL_TASK = "#4a90d9"

WORLD_COLOURS = {
    "cache": "#4a90d9",
    "sparse": "#7ed17e",
    "unordered": "#e8a838",
}


# --- Data Loading ---

def load_profile(path):
    if os.path.getsize(path) == 0:
        print(f"  Skipping {path} — empty file")
        return None
    with open(path) as f:
        try:
            return json.load(f)
        except json.JSONDecodeError as e:
            print(f"  Skipping {path} — invalid JSON: {e}")
            return None


def normalise_time(timestamps):
    t0 = timestamps[0]
    return [(t - t0) / 1000.0 for t in timestamps]


def moving_average(data, window=15):
    if len(data) < window:
        return data
    return np.convolve(data, np.ones(window) / window, mode="valid")


def extract_world_type(path):
    name = Path(path).stem.lower()
    for wt in ["cache", "sparse", "unordered"]:
        if wt in name:
            return wt.capitalize()
    return "Unknown"


def extract_config(path):
    """Extract config label like c32_r8 from path."""
    parts = Path(path).parts
    for p in parts:
        if p.startswith("c") and "_r" in p:
            return p
    return Path(path).stem


def compute_stats(samples, tasks):
    update_ms = [s["update_ms"] for s in samples]
    render_ms = [s["render_ms"] for s in samples]
    total_ms = [s["total_ms"] for s in samples]
    total_chunks = [s["total_chunks"] for s in samples]
    memory_bytes = [s["memory_bytes"] for s in samples]
    task_dur = [t["duration_ms"] for t in tasks] if tasks else []

    return {
        "count": len(samples),
        "task_count": len(tasks),
        "avg_update_ms": np.mean(update_ms),
        "avg_render_ms": np.mean(render_ms),
        "avg_total_ms": np.mean(total_ms),
        "max_update_ms": np.max(update_ms),
        "max_render_ms": np.max(render_ms),
        "avg_chunks": np.mean(total_chunks),
        "avg_memory_kb": np.mean(memory_bytes) / 1024,
        "avg_task_ms": np.mean(task_dur) if task_dur else 0,
        "max_task_ms": np.max(task_dur) if task_dur else 0,
        "update_ms": update_ms,
        "render_ms": render_ms,
        "total_ms": total_ms,
        "total_chunks": total_chunks,
        "memory_bytes": memory_bytes,
        "task_dur": task_dur,
        "timestamps": normalise_time([s["timestamp_ms"] for s in samples]),
    }


# --- Matplotlib Graphs → PNG bytes ---

def fig_to_bytes(fig):
    buf = io.BytesIO()
    fig.savefig(buf, format="png", dpi=130, bbox_inches="tight")
    plt.close(fig)
    buf.seek(0)
    return buf


def graph_timing(stats_list, config):
    fig, ax = plt.subplots(figsize=(12, 4))
    fig.suptitle(f"Update & Render Timing — {config}", fontsize=13, fontweight="bold")

    for st, label, col in stats_list:
        t = st["timestamps"]
        window = 15
        t_avg = t[window - 1:]
        ax.plot(t_avg, moving_average(st["update_ms"], window),
                label=f"{label} update", color=WORLD_COLOURS.get(label.lower(), "#888"), linewidth=1.4)
        ax.plot(t_avg, moving_average(st["render_ms"], window),
                label=f"{label} render", color=WORLD_COLOURS.get(label.lower(), "#888"),
                linewidth=1.4, linestyle="--", alpha=0.7)

    ax.set_xlabel("Time (s)")
    ax.set_ylabel("ms")
    ax.legend(fontsize=8)
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    return fig_to_bytes(fig)


def graph_chunks_memory(stats_list, config):
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 4))
    fig.suptitle(f"Chunks & Memory — {config}", fontsize=13, fontweight="bold")

    for st, label, _ in stats_list:
        col = WORLD_COLOURS.get(label.lower(), "#888")
        ax1.plot(st["timestamps"], st["total_chunks"], label=label, color=col, linewidth=1.2)
        ax2.plot(st["timestamps"], [m / 1024 for m in st["memory_bytes"]], label=label, color=col, linewidth=1.2)

    ax1.set_title("Active Chunks")
    ax1.set_xlabel("Time (s)")
    ax1.set_ylabel("Chunks")
    ax1.legend(fontsize=8)
    ax1.grid(True, alpha=0.3)

    ax2.set_title("Memory Usage")
    ax2.set_xlabel("Time (s)")
    ax2.set_ylabel("KB")
    ax2.legend(fontsize=8)
    ax2.grid(True, alpha=0.3)

    fig.tight_layout()
    return fig_to_bytes(fig)


def graph_task_histogram(stats_list, config):
    # Only include worlds that have task data
    has_tasks = [(st, label, col) for st, label, col in stats_list if st["task_dur"]]
    if not has_tasks:
        return None

    fig, axes = plt.subplots(1, len(has_tasks), figsize=(6 * len(has_tasks), 4))
    fig.suptitle(f"Task Duration Distribution — {config}", fontsize=13, fontweight="bold")

    if len(has_tasks) == 1:
        axes = [axes]

    for ax, (st, label, _) in zip(axes, has_tasks):
        td = st["task_dur"]
        col = WORLD_COLOURS.get(label.lower(), "#888")
        ax.hist(td, bins=60, color=col, edgecolor="none", alpha=0.8)
        ax.set_xlim(0, np.percentile(td, 99))
        ax.axvline(np.mean(td), color="#e85454", linestyle="--", linewidth=1.5,
                   label=f"Mean: {np.mean(td):.2f}ms")
        ax.axvline(np.median(td), color="#333", linestyle="--", linewidth=1.5,
                   label=f"Median: {np.median(td):.2f}ms")
        ax.set_title(label)
        ax.set_xlabel("Duration (ms)")
        ax.set_ylabel("Count")
        ax.legend(fontsize=8)
        ax.grid(True, alpha=0.3)

    fig.tight_layout()
    return fig_to_bytes(fig)


def graph_comparison_bar(all_configs):
    """Bar chart comparing avg update_ms across all configs and world types."""
    configs = sorted(all_configs.keys())
    worlds = ["Cache", "Sparse", "Unordered"]
    x = np.arange(len(configs))
    width = 0.25

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
    fig.suptitle("Cross-Config Comparison", fontsize=13, fontweight="bold")

    for i, world in enumerate(worlds):
        update_avgs = []
        memory_avgs = []
        for cfg in configs:
            entry = all_configs[cfg].get(world.lower())
            update_avgs.append(entry["avg_update_ms"] if entry else 0)
            memory_avgs.append(entry["avg_memory_kb"] if entry else 0)

        col = WORLD_COLOURS.get(world.lower(), "#888")
        ax1.bar(x + i * width, update_avgs, width, label=world, color=col, alpha=0.85)
        ax2.bar(x + i * width, memory_avgs, width, label=world, color=col, alpha=0.85)

    ax1.set_title("Avg Update Time")
    ax1.set_xlabel("Config")
    ax1.set_ylabel("ms")
    ax1.set_xticks(x + width)
    ax1.set_xticklabels(configs)
    ax1.legend()
    ax1.grid(True, alpha=0.3, axis="y")

    ax2.set_title("Avg Memory Usage")
    ax2.set_xlabel("Config")
    ax2.set_ylabel("KB")
    ax2.set_xticks(x + width)
    ax2.set_xticklabels(configs)
    ax2.legend()
    ax2.grid(True, alpha=0.3, axis="y")

    fig.tight_layout()
    return fig_to_bytes(fig)


# --- ReportLab PDF Builder ---

def build_pdf(output_path, all_configs):
    doc = SimpleDocTemplate(
        output_path,
        pagesize=A4,
        leftMargin=1.5 * cm, rightMargin=1.5 * cm,
        topMargin=1.5 * cm, bottomMargin=1.5 * cm
    )

    styles = getSampleStyleSheet()
    title_style = ParagraphStyle("title", fontSize=20, fontName="Helvetica-Bold",
                                 alignment=TA_CENTER, spaceAfter=6)
    h1_style = ParagraphStyle("h1", fontSize=14, fontName="Helvetica-Bold",
                              spaceBefore=12, spaceAfter=6)
    h2_style = ParagraphStyle("h2", fontSize=11, fontName="Helvetica-Bold",
                              spaceBefore=8, spaceAfter=4)
    body_style = styles["Normal"]
    small_style = ParagraphStyle("small", fontSize=8, fontName="Helvetica",
                                 textColor=colors.grey)

    page_w = A4[0] - 3 * cm
    story = []

    # ── Title page ──
    story.append(Spacer(1, 1 * cm))
    story.append(Paragraph("Project Life — Performance Report", title_style))
    story.append(Paragraph("Cellular Automaton World Container Benchmark",
                           ParagraphStyle("sub", fontSize=11, alignment=TA_CENTER,
                                          textColor=colors.grey, spaceAfter=20)))
    story.append(HRFlowable(width="100%", thickness=1, color=colors.lightgrey))
    story.append(Spacer(1, 0.5 * cm))

    configs = sorted(all_configs.keys())

    # --- Per-config sections ---
    for cfg in configs:
        story.append(Paragraph(f"Configuration: {cfg}", h1_style))
        story.append(HRFlowable(width="100%", thickness=0.5, color=colors.lightgrey))
        story.append(Spacer(1, 0.2 * cm))

        worlds_in_cfg = all_configs[cfg]
        stats_list = [
            (st, wt.capitalize(), WORLD_COLOURS.get(wt, "#888"))
            for wt, st in worlds_in_cfg.items()
        ]

        # Timing graph
        img_bytes = graph_timing(stats_list, cfg)
        story.append(Image(img_bytes, width=page_w, height=page_w * 0.33))
        story.append(Spacer(1, 0.2 * cm))

        # Chunks + memory
        img_bytes = graph_chunks_memory(stats_list, cfg)
        story.append(Image(img_bytes, width=page_w, height=page_w * 0.33))
        story.append(Spacer(1, 0.2 * cm))

        # Task histogram
        img_bytes = graph_task_histogram(stats_list, cfg)
        if img_bytes:
            story.append(Image(img_bytes, width=page_w, height=page_w * 0.33))
            story.append(Spacer(1, 0.2 * cm))

        # Per-world summary table
        story.append(Paragraph("Summary", h2_style))

        table_data = [["World", "Avg Update", "Avg Render", "Avg Total",
                       "Max Update", "Avg Chunks", "Avg Memory", "Samples"]]

        for wt, st in worlds_in_cfg.items():
            table_data.append([
                wt.capitalize(),
                f"{st['avg_update_ms']:.2f} ms",
                f"{st['avg_render_ms']:.2f} ms",
                f"{st['avg_total_ms']:.2f} ms",
                f"{st['max_update_ms']:.2f} ms",
                f"{st['avg_chunks']:.0f}",
                f"{st['avg_memory_kb']:.1f} KB",
                str(st["count"]),
            ])

        t = Table(table_data, repeatRows=1)
        t.setStyle(TableStyle([
            ("BACKGROUND", (0, 0), (-1, 0), colors.HexColor("#2d2d2d")),
            ("TEXTCOLOR", (0, 0), (-1, 0), colors.white),
            ("FONTNAME", (0, 0), (-1, 0), "Helvetica-Bold"),
            ("FONTSIZE", (0, 0), (-1, -1), 8),
            ("ROWBACKGROUNDS", (0, 1), (-1, -1), [colors.HexColor("#f5f5f5"), colors.white]),
            ("GRID", (0, 0), (-1, -1), 0.5, colors.lightgrey),
            ("ALIGN", (1, 0), (-1, -1), "CENTER"),
            ("TOPPADDING", (0, 0), (-1, -1), 4),
            ("BOTTOMPADDING", (0, 0), (-1, -1), 4),
        ]))
        story.append(t)
        story.append(PageBreak())

    # --- Cross-config comparison ---
    story.append(Paragraph("Cross-Configuration Comparison", h1_style))
    story.append(HRFlowable(width="100%", thickness=0.5, color=colors.lightgrey))
    story.append(Spacer(1, 0.3 * cm))

    img_bytes = graph_comparison_bar(all_configs)
    story.append(Image(img_bytes, width=page_w, height=page_w * 0.36))
    story.append(Spacer(1, 0.5 * cm))

    # ── Final summary table ──
    story.append(Paragraph("Overall Summary", h1_style))
    story.append(HRFlowable(width="100%", thickness=0.5, color=colors.lightgrey))
    story.append(Spacer(1, 0.3 * cm))

    summary_data = [["Config", "World", "Avg Update ms", "Avg Render ms",
                     "Avg Memory KB", "Avg Chunks", "Tasks"]]

    for cfg in configs:
        for wt, st in all_configs[cfg].items():
            summary_data.append([
                cfg, wt.capitalize(),
                f"{st['avg_update_ms']:.2f}",
                f"{st['avg_render_ms']:.2f}",
                f"{st['avg_memory_kb']:.1f}",
                f"{st['avg_chunks']:.0f}",
                str(st["task_count"]),
            ])

    t = Table(summary_data, repeatRows=1)
    t.setStyle(TableStyle([
        ("BACKGROUND", (0, 0), (-1, 0), colors.HexColor("#2d2d2d")),
        ("TEXTCOLOR", (0, 0), (-1, 0), colors.white),
        ("FONTNAME", (0, 0), (-1, 0), "Helvetica-Bold"),
        ("FONTSIZE", (0, 0), (-1, -1), 8),
        ("ROWBACKGROUNDS", (0, 1), (-1, -1), [colors.HexColor("#f5f5f5"), colors.white]),
        ("GRID", (0, 0), (-1, -1), 0.5, colors.lightgrey),
        ("ALIGN", (1, 0), (-1, -1), "CENTER"),
        ("TOPPADDING", (0, 0), (-1, -1), 4),
        ("BOTTOMPADDING", (0, 0), (-1, -1), 4),
    ]))
    story.append(t)
    story.append(Spacer(1, 0.5 * cm))

    # --- Footer notes ---
    story.append(HRFlowable(width="100%", thickness=0.5, color=colors.lightgrey))
    story.append(Spacer(1, 0.2 * cm))
    story.append(Paragraph(
        "Notes: Update ms = time to process one simulation step across all chunks. "
        "Render ms = time to draw the world each frame. "
        "Moving average (window=15) applied to timing graphs to reduce noise. "
        "Task data not recorded for Unordered world type due to volume.",
        small_style
    ))

    doc.build(story)
    print(f"Saved: {output_path}")


# --- File Discovery ---

def discover_profiles(root):
    """
    Walk the output folder and group JSON files by config and world type.
    Returns: { "c32_r8": { "cache": stats, "sparse": stats, ... }, ... }
    """
    all_configs = {}

    for path in sorted(glob.glob(f"{root}/**/*.json", recursive=True)):
        name = Path(path).stem.lower()
        world_type = None
        for wt in ["cache", "sparse", "unordered"]:
            if wt in name:
                world_type = wt
                break
        if not world_type:
            continue

        config = extract_config(path)

        data = load_profile(path)
        if data is None:
            continue

        samples = data.get("samples", [])
        tasks = data.get("tasks", [])

        if not samples:
            print(f"  Skipping {path} — no samples")
            continue

        print(f"  Loaded {len(samples)} samples, {len(tasks)} tasks — {path}")
        all_configs.setdefault(config, {})[world_type] = compute_stats(samples, tasks)

    return all_configs


# --- Entry Point ---

if __name__ == "__main__":
    target = sys.argv[1] if len(sys.argv) > 1 else "output"

    if os.path.isdir(target):
        print(f"Scanning {target}...")
        all_configs = discover_profiles(target)

        if not all_configs:
            print("No profile JSON files found.")
            sys.exit(1)

        out_pdf = "performance_report.pdf"
        build_pdf(out_pdf, all_configs)

    elif os.path.isfile(target) and target.endswith(".json"):
        # Single file — wrap in a minimal structure
        data = load_profile(target)
        samples = data.get("samples", [])
        tasks = data.get("tasks", [])
        wt = extract_world_type(target).lower()
        cfg = extract_config(target)
        stats = compute_stats(samples, tasks)
        all_configs = {cfg: {wt: stats}}
        build_pdf("performance_report.pdf", all_configs)

    else:
        print(f"Usage: ./generate_report.py output/   or   ./generate_report.py profile.json")
