#!/usr/bin/env bash
set -x
here=$(cd "$(dirname "$0")" && pwd)
me=$(basename "$0")

IMAGE=chip-run-amd64

help() {
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

RUN_DIR_HOST="$here/../../"
RUN_DIR_DOCKER="/connectedhomeip/"
docker run --network=host --platform linux/amd64 -d --rm \
  "${runargs[@]}" --name "$IMAGE" --privileged \
  --mount "source=/var/run/docker.sock,target=/var/run/docker.sock,type=bind" \
  -v "/dev/:/dev" \
  -w "$RUN_DIR_DOCKER" \
  -v "$RUN_DIR_HOST:$RUN_DIR_DOCKER" "$IMAGE" "$@"
