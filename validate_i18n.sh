#!/usr/bin/env sh
set -eu

LOCALES_DIR="./frontend/dictionary/i18n"
BASE_LANG="en.json"

BASE_FILE="$LOCALES_DIR/$BASE_LANG"

# Implicitly allowed EN values (brand / product names)
IMPLICIT_ALLOW_EN_KEYS="
dictionary.button.mindnet
"

if [ ! -f "$BASE_FILE" ]; then
  echo "❌ ERROR #1 Base language file not found: $BASE_FILE"
  exit 1
fi

JSON_NUMBER=0

FAILED=0
ISSUE_NO=0

LANGUAGES_CHECKED=0          # NEW
LANGUAGES_WITH_MISSING=0     # NEW
TOTAL_MISSING_KEYS=0         # NEW

# All translation keys except allow_en
BASE_KEYS=$(jq -r 'keys[] | select(. != "allow_en")' "$BASE_FILE")
TOTAL_KEYS=$(printf "%s\n" "$BASE_KEYS" | wc -l | tr -d ' ')

for FILE in "$LOCALES_DIR"/*.json; do
  NAME=$(basename "$FILE")
  [ "$NAME" = "$BASE_LANG" ] && continue

  LANG="${NAME%.json}"
  echo ""
  JSON_NUMBER=$((JSON_NUMBER + 1))
  LANGUAGES_CHECKED=$((LANGUAGES_CHECKED + 1))   # NEW
  echo "🔍 Validating #$JSON_NUMBER $NAME"

  # language-specific whitelist (allow_en)
  ALLOW_EN_KEYS=$(jq -r '.allow_en[]? // empty' "$FILE")

  MISSING_KEYS=0

  # 1) Missing keys → ERROR
  for KEY in $BASE_KEYS; do
    if ! jq -e --arg k "$KEY" 'has($k)' "$FILE" >/dev/null; then
      ISSUE_NO=$((ISSUE_NO + 1))
      MISSING_KEYS=$((MISSING_KEYS + 1))
      TOTAL_MISSING_KEYS=$((TOTAL_MISSING_KEYS + 1))   # NEW

      LINE_NO=$(grep -n "\"$KEY\"" "$BASE_FILE" | head -n 1 | cut -d: -f1)
      [ -z "$LINE_NO" ] && LINE_NO="?"

      echo "❌ ERROR   #$ISSUE_NO [$LANG] missing key: $KEY at line $LINE_NO"
      FAILED=1
    fi
  done

  if [ "$MISSING_KEYS" -gt 0 ]; then
    LANGUAGES_WITH_MISSING=$((LANGUAGES_WITH_MISSING + 1))   # NEW
  fi

  # 2) Extra keys → WARNING
  for KEY in $(jq -r 'keys[] | select(. != "allow_en")' "$FILE"); do
    if ! jq -e --arg k "$KEY" 'has($k)' "$BASE_FILE" >/dev/null; then
      ISSUE_NO=$((ISSUE_NO + 1))
      echo "⚠️  WARNING #$ISSUE_NO [$LANG] extra key: $KEY"
    fi
  done

  # 3) Values identical to EN
  for KEY in $BASE_KEYS; do
    echo "$IMPLICIT_ALLOW_EN_KEYS" | grep -qx "$KEY" && continue

    BASE_VAL=$(jq -r --arg k "$KEY" '.[$k]' "$BASE_FILE")
    LANG_VAL=$(jq -r --arg k "$KEY" '.[$k]' "$FILE")

    if echo "$ALLOW_EN_KEYS" | grep -qx "$KEY"; then
      if [ "$BASE_VAL" != "$LANG_VAL" ]; then
        ISSUE_NO=$((ISSUE_NO + 1))
        echo "⚠️  WARNING #$ISSUE_NO [$LANG] key in allow_en but value differs from EN: $KEY"
      fi
      continue
    fi

    if [ "$BASE_VAL" = "$LANG_VAL" ]; then
      ISSUE_NO=$((ISSUE_NO + 1))
      echo "⚠️  WARNING #$ISSUE_NO [$LANG] untranslated (same as EN): $KEY"
    fi
  done

  # Completeness per language
  if [ "$MISSING_KEYS" -gt 0 ]; then
    COMPLETENESS=$(( (TOTAL_KEYS - MISSING_KEYS) * 100 / TOTAL_KEYS ))
    echo "❌ COMPLETENESS [$LANG]: ${COMPLETENESS}% (${MISSING_KEYS}/${TOTAL_KEYS} missing)"
  fi
done

# SUMMARY  # NEW
echo ""
echo "SUMMARY:"
echo "- languages checked: $LANGUAGES_CHECKED"
echo "- languages with missing keys: $LANGUAGES_WITH_MISSING"
echo "- total missing keys: $TOTAL_MISSING_KEYS"

if [ "$FAILED" -ne 0 ]; then
  echo ""
  echo "🚨 i18n validation FAILED"
  exit 1
fi

echo ""
echo "✅ i18n validation OK"
