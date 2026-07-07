#!/usr/bin/env bash

ENV_VARS=()

# Process .env file
process-envfile() {
	local line
	while IFS= read -r line; do
		if [[ -z "$line" || "$line" =~ ^# ]]; then
			continue;
		fi
		ENV_VARS+=("$line")
	done
}

print-vars() {
	printf "%s\n" "${ENV_VARS[@]}" | xargs
}

# Entry Point
main() {
	while [[ $# -gt 0 ]]; do
		if [[ -f "$1" ]]; then
			process-envfile < "$1"
		else
			echo The .env file at "$1" does not exist >&2
		fi

		shift 
	done

	print-vars
}

main "$@"
