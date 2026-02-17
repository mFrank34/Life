import os
import subprocess
from datetime import date

# === CONFIG ===
PROJECT_DIR = "~/Projects/Life/src"  # your project root
PROJECT_DIR = os.path.expanduser(PROJECT_DIR)
AUTHOR = "Michael Franks"

HEADER_TEMPLATE = """\
/*
 * File: {filename}
 * Author: {author}
 * Created: {created}
 * Last Edited: {last_edited}
 * Description:
 */
"""


def git_file_dates(file_path):
    """
    Returns (created_date, last_edited_date) from git history.
    Dates are YYYY-MM-DD strings.
    """
    try:
        # Created date: first commit that added the file
        created_ts = subprocess.check_output(
            ["git", "log", "--diff-filter=A", "--follow", "--format=%ad", "--date=short", file_path],
            cwd=PROJECT_DIR,
            encoding="utf-8"
        ).splitlines()[0]

        # Last edited date: last commit that modified the file
        last_ts = subprocess.check_output(
            ["git", "log", "-1", "--format=%ad", "--date=short", file_path],
            cwd=PROJECT_DIR,
            encoding="utf-8"
        ).strip()

        return created_ts, last_ts
    except Exception:
        # Fallback: use today for both if git info unavailable
        today = date.today().strftime("%Y-%m-%d")
        return today, today


# === SCRIPT ===
for root, dirs, files in os.walk(PROJECT_DIR):
    for filename in files:
        if filename.endswith((".cpp", ".h")):
            rel_path = os.path.relpath(os.path.join(root, filename), PROJECT_DIR)
            file_path = os.path.join(root, filename)

            created_date, last_edited = git_file_dates(rel_path)

            with open(file_path, "r+", encoding="utf-8") as f:
                content = f.read()
                lines = content.splitlines()

                # Check if header exists
                header_exists = any("Author:" in line for line in lines[:10])

                if header_exists:
                    # Update Last Edited line if it exists
                    for i, line in enumerate(lines[:10]):
                        if "Last Edited:" in line:
                            lines[i] = f" * Last Edited: {last_edited}"
                            break
                    else:
                        # If Last Edited line not found, insert after Created
                        for i, line in enumerate(lines[:10]):
                            if "Created:" in line:
                                lines.insert(i + 1, f" * Last Edited: {last_edited}")
                                break

                    f.seek(0)
                    f.write("\n".join(lines))
                    f.truncate()
                else:
                    # Prepend full header
                    header = HEADER_TEMPLATE.format(
                        filename=filename,
                        author=AUTHOR,
                        created=created_date,
                        last_edited=last_edited
                    )
                    f.seek(0)
                    f.write(header + "\n" + content)

print("Headers updated using Git: Created and Last Edited dates populated.")
