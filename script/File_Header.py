import os
import re

# === CONFIG ===
PROJECT_DIR = "~/Projects/Life/src"
PROJECT_DIR = os.path.expanduser(PROJECT_DIR)
AUTHOR = "Michael Franks"

HEADER_TEMPLATE = """\
/*
 * File: {filename}
 * Author: {author}
 * Description:
 */
"""


def process_file(file_path):
    with open(file_path, "r+", encoding="utf-8") as f:
        content = f.read()

        # Regex to match an existing header at the top
        # Matches /* ... */ block if it contains 'Author:'
        header_regex = r"(?s)^/\*.*?Author:.*?\*/\s*"
        new_content = re.sub(header_regex, "", content, count=1)

        # Prepend new header
        header = HEADER_TEMPLATE.format(filename=os.path.basename(file_path), author=AUTHOR)
        f.seek(0)
        f.write(header + "\n" + new_content)
        f.truncate()


# Walk project recursively
for root, dirs, files in os.walk(PROJECT_DIR):
    for fn in files:
        if fn.endswith((".cpp", ".h")):
            process_file(os.path.join(root, fn))

print("All headers rewritten with minimal format.")
