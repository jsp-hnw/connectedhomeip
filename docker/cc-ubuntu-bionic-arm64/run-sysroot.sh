#!/usr/bin/env bash
set -x
here=$(cd "$(dirname "$0")" && pwd)
me=$(basename "$0")

IMAGE=$(basename $here)-sysroot
OPT_ROOT=/opt/$(basename $here)

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

context=$(docker context show)
if [ "$context" == "default" ]; then
    user=$USER
    home=$HOME
else # if docker desktop
    user=root
    home=/root
fi

docker run --platform=linux/arm64 -it --rm --user $user \
    "${runargs[@]}" --privileged \
    -w "$RUN_DIR_DOCKER" \
    -v "$RUN_DIR_HOST:$RUN_DIR_DOCKER" \
    "$IMAGE" "$@"
