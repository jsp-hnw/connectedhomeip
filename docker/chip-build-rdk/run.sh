#!/usr/bin/env bash

#
#    Copyright (c) 2020 Project CHIP Authors
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

# run.sh   - utility for running a Docker image
#
# This script expects to live in a directory named after the image
#  with a version file next to it.  So: use symlinks
#
set -x
here=$(cd "$(dirname "$0")" && pwd)
me=$(basename "$0")

die() {
    echo "$me: *** ERROR: $*"
    exit 1
}

ORG=${DOCKER_RUN_ORG:-project-chip}

GHCR_ORG="ghcr.io"

FULL_IMAGE_NAME=$(basename "$(pwd)")

# version
    set +x
    echo "Usage: $me [RUN_OPTIONS -- ] command

  Run a command in a docker image described by $here

  Options:
   --help        get this message

  Any number of 'docker run' options can be passed
     through to the invocation.  Terminate this list of
     options with '--' to begin command and arguments.

  Examples:
    To run bash interactively:
      $ $me -i -- bash
     note the terminating '--' for run options

    To just tell me about the image
      $ $me uname -a

    Add /tmp as an additional volume and run make
      $ $me --volume /tmp:/tmp -- make -C src

"

}

runargs=()

# extract run options
for arg in "$@"; do
    case "$arg" in
    --help)
        help
        exit
        ;;

    --)
        shift
        break
        ;;

    -*)
        runargs+=("$arg")
        shift
        ;;

    *)
        ((!${#runargs[*]})) && break
        runargs+=("$arg")
        shift
        ;;

    esac
done

###docker pull "$FULL_IMAGE_NAME" || "$here"/build.sh
RUN_DIR_HOST="$here/../../"
RUN_DIR_DOCKER="/connectedhomeip/"

context=$(docker context show)
if [ "$context" == "default" ]; then
    uid=$(id -u)
    gid=$(id -g)
else # if docker desktop
    uid=root
    gid=root
fi

docker run "${runargs[@]}" --rm -it --user $uid:$gid \
    --mount "source=/var/run/docker.sock,target=/var/run/docker.sock,type=bind" \
    -w "$RUN_DIR_DOCKER" -v "/opt:/opt" -v "$RUN_DIR_HOST:$RUN_DIR_DOCKER" "$FULL_IMAGE_NAME" "$@"
