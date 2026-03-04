#!/usr/bin/env python3

"""
Profile Graph Generator
Author: Michael Franks
Description: Generates performance graphs from profile JSON data
Usage: python3 profile_graphs.py profile_Cache.json
"""

import json
import os
import sys
from pathlib import Path

import matplotlib.gridspec as gridspec
import matplotlib.pyplot as plt
import numpy as np


# --- Load Data ---

def load_profile(path):
    with open(path) as f:
        return json.load(f)


# --- Helpers ---

def normalise_time(timestamps):
    t0 = timestamps[0]
    return [(t - t0) / 1000.0 for t in timestamps]  # ms -> seconds


def moving_average(data, window=20):
    return np.convolve(data, np.ones(window) / window, mode='valid')


# --- Plots ---

def plot_timing(ax, time_s, update_ms, render_ms, total_ms, label):
    window = 20
    t = time_s[window - 1:]

    ax.plot(t, moving_average(update_ms, window), label="Update ms", color="#4a90d9", linewidth=1.2)
    ax.plot(t, moving_average(render_ms, window), label="Render ms", color="#e8a838", linewidth=1.2)
    ax.plot(t, moving_average(total_ms, window), label="Total ms", color="#e85454", linewidth=1.2, linestyle="--")

    ax.set_title(f"Timing Over Time — {label}")
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("ms")
    ax.legend()
    ax.grid(True, alpha=0.3)


def plot_chunks(ax, time_s, total_chunks, label):
    ax.plot(time_s, total_chunks, color="#7ed17e", linewidth=1.2)
    ax.set_title(f"Active Chunks Over Time — {label}")
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Chunks")
    ax.grid(True, alpha=0.3)


def plot_memory(ax, time_s, memory_bytes, label):
    memory_kb = [m / 1024 for m in memory_bytes]
    ax.plot(time_s, memory_kb, color="#b07adb", linewidth=1.2)
    ax.set_title(f"Memory Usage Over Time — {label}")
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("KB")
    ax.grid(True, alpha=0.3)


def plot_task_histogram(ax, task_durations, label):
    if not task_durations:
        ax.text(0.5, 0.5, "No task data", ha="center", va="center", transform=ax.transAxes)
        return

    ax.hist(task_durations, bins=60, color="#4a90d9", edgecolor="none", alpha=0.8)
    ax.set_xlim(0, np.percentile(task_durations, 99))  # cut off top 1% outliers
    ax.axvline(np.mean(task_durations), color="#e85454", linestyle="--", linewidth=1.5,
               label=f"Mean: {np.mean(task_durations):.2f}ms")
    ax.axvline(np.median(task_durations), color="#e8a838", linestyle="--", linewidth=1.5,
               label=f"Median: {np.median(task_durations):.2f}ms")
    ax.set_title(f"Task Duration Distribution — {label}")
    ax.set_xlabel("Duration (ms)")
    ax.set_ylabel("Count")
    ax.legend()
    ax.grid(True, alpha=0.3)


def plot_thread_boxplot(ax, tasks, label):
    if not tasks:
        ax.text(0.5, 0.5, "No task data", ha="center", va="center", transform=ax.transAxes)
        return

    # Group by thread id
    thread_map = {}
    for t in tasks:
        tid = t["thread_id"]
        thread_map.setdefault(tid, []).append(t["duration_ms"])

    thread_ids = sorted(thread_map.keys())
    data = [thread_map[tid] for tid in thread_ids]

    ax.boxplot(data, labels=[f"T{tid}" for tid in thread_ids], patch_artist=True,
               boxprops=dict(facecolor="#4a90d9", alpha=0.7),
               medianprops=dict(color="#e85454", linewidth=2))
    ax.set_title(f"Task Duration per Thread — {label}")
    ax.set_xlabel("Thread")
    ax.set_ylabel("Duration (ms)")
    ax.grid(True, alpha=0.3, axis="y")


# --- Main ---

def generate_graphs(path):
    data = load_profile(path)
    label = Path(path).stem  # e.g. "profile_Cache"
    samples = data["samples"]
    tasks = data.get("tasks", [])

    if not samples:
        print(f"No samples found in {path}")
        return

    # Extract sample fields
    timestamps = [s["timestamp_ms"] for s in samples]
    update_ms = [s["update_ms"] for s in samples]
    render_ms = [s["render_ms"] for s in samples]
    total_ms = [s["total_ms"] for s in samples]
    total_chunks = [s["total_chunks"] for s in samples]
    memory_bytes = [s["memory_bytes"] for s in samples]
    time_s = normalise_time(timestamps)

    task_durations = [t["duration_ms"] for t in tasks] if tasks else []

    # ── Figure layout ──
    fig = plt.figure(figsize=(18, 14))
    fig.suptitle(f"Performance Profile — {label}", fontsize=16, fontweight="bold", y=0.98)

    gs = gridspec.GridSpec(3, 2, figure=fig, hspace=0.45, wspace=0.3)

    ax1 = fig.add_subplot(gs[0, :])  # timing — full width
    ax2 = fig.add_subplot(gs[1, 0])  # chunks
    ax3 = fig.add_subplot(gs[1, 1])  # memory
    ax4 = fig.add_subplot(gs[2, 0])  # task histogram
    ax5 = fig.add_subplot(gs[2, 1])  # thread boxplot

    plot_timing(ax1, time_s, update_ms, render_ms, total_ms, label)
    plot_chunks(ax2, time_s, total_chunks, label)
    plot_memory(ax3, time_s, memory_bytes, label)
    plot_task_histogram(ax4, task_durations, label)
    plot_thread_boxplot(ax5, tasks, label)

    # --- Stats summary ---
    print(f"\n── {label} ──────────────────────────────")
    print(f"  Samples:        {len(samples)}")
    print(f"  Tasks:          {len(tasks)}")
    print(f"  Duration:       {time_s[-1]:.1f}s")
    print(f"  Avg update ms:  {np.mean(update_ms):.2f}")
    print(f"  Avg render ms:  {np.mean(render_ms):.2f}")
    print(f"  Avg total ms:   {np.mean(total_ms):.2f}")
    print(f"  Avg chunks:     {np.mean(total_chunks):.0f}")
    print(f"  Avg memory KB:  {np.mean(memory_bytes) / 1024:.1f}")
    if task_durations:
        print(f"  Avg task ms:    {np.mean(task_durations):.2f}")
        print(f"  Max task ms:    {np.max(task_durations):.2f}")

    # ── Save ──
    out_path = Path(path).stem + "_graphs.png"
    plt.savefig(out_path, dpi=150, bbox_inches="tight")
    print(f"\n  Saved: {out_path}")
    plt.close()


if __name__ == "__main__":
    paths = sys.argv[1:] if len(sys.argv) > 1 else ["profile.json"]
    for p in paths:
        if os.path.exists(p):
            generate_graphs(p)
        else:
            print(f"File not found: {p}")
