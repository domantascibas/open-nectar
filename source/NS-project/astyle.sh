#!/bin/bash

if ! hash astyle 2>/dev/null; then
    echo "Error: 'astyle' was not found in PATH"
	exit 1
fi

if [ $# -lt 2 ]; then
	echo "Error: missing arguments"
	exit 1
fi

RUN="${1}"
SRCDIR="${2}"
COMMAND="astyle --options=../../.astylerc --suffix=none --formatted --recursive "${SRCDIR}"/*.c,*.h"

if [ "${RUN}" = "CHECK" ]; then
	echo "Checking code..."
	OUTPUT="$(${COMMAND} --dry-run)"
	echo "${OUTPUT}"
	if [[ $OUTPUT = *"Formatted"* ]]; then
	  echo "${OUTPUT}"
	  echo "------------------------------------------------------------"
	  echo "Error: unformatted code"
	  echo "------------------------------------------------------------"
	  exit 1
	fi
else
	echo "Formatting code..."
	OUTPUT="$(${COMMAND})"
	echo "${OUTPUT}"
fi
	