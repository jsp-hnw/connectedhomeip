#!/usr/bin/env bash

me=$(basename "$0")
cd "$(dirname "$0")"

IMAGE=$(basename "$(pwd)")

set -ex

docker buildx build \
    -t "$IMAGE" \
    --build-arg USER_UID="$UID" \
    --build-arg USERNAME="$USER" \
    --build-arg SDK_ROOT=/opt/$(basename "$(pwd)") .
docker image prune --force

exit 0
