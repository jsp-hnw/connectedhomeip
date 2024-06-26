#!/usr/bin/env bash

me=$(basename "$0")
cd "$(dirname "$0")"

IMAGE=chip-run-arm64

[[ ${*/--help//} != "${*}" ]] && {
    set +x
    echo "Usage: $me <OPTIONS>

  Build and (optionally tag as latest, push) a docker image from Dockerfile in CWD

  Options:
   --no-cache   passed as a docker build argument
   --help       get this message
"
    exit 0
}

set -ex

BUILD_ARGS=()
if [[ ${*/--no-cache//} != "${*}" ]]; then
    BUILD_ARGS+=(--no-cache)
fi

[[ ${*/--skip-build//} != "${*}" ]] || {
    docker buildx build --platform linux/arm64 "${BUILD_ARGS[@]}" -t "$IMAGE" .
    docker image prune --force
}

exit 0
