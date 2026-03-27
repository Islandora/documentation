#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
GLOSSARY_FILE="${1:-$ROOT_DIR/docs/user-documentation/glossary.md}"
OUTPUT_FILE="${2:-$ROOT_DIR/includes/abbreviations.md}"
TARGET_PATH="../user-documentation/glossary.md"

declare -A seen_labels=()

if [[ ! -f "$GLOSSARY_FILE" ]]; then
  printf 'Glossary file not found: %s\n' "$GLOSSARY_FILE" >&2
  exit 1
fi

mkdir -p "$(dirname "$OUTPUT_FILE")"

slugify() {
  local value="$1"

  value="$(printf '%s' "$value" | tr '[:upper:]' '[:lower:]')"
  value="$(printf '%s' "$value" | sed -E 's/[^a-z0-9 _-]+//g')"
  value="$(printf '%s' "$value" | sed -E 's/[[:space:]_-]+/-/g; s/^-+//; s/-+$//')"

  printf '%s\n' "$value"
}

emit_label() {
  local label="$1"
  local slug="$2"

  [[ -n $label ]] || return
  if [[ -v "seen_labels[$label]" ]]; then
    return
  fi

  printf '[%s]: %s#%s\n' "$label" "$TARGET_PATH" "$slug"
  seen_labels["$label"]=1
}

pluralize() {
  local value="$1"

  if [[ $value =~ [sS]$ ]]; then
    printf '%s\n' "$value"
    return
  fi

  printf '%ss\n' "$value"
}

while IFS= read -r line; do
  [[ $line == '## '* ]] || continue

  heading="${line#\#\# }"
  slug="$(slugify "$heading")"

  [[ -n $slug ]] || continue

  emit_label "$heading" "$slug"
  emit_label "$(pluralize "$heading")" "$slug"
done < "$GLOSSARY_FILE" > "$OUTPUT_FILE"
