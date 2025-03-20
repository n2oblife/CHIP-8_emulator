#!/bin/bash

if [ "$(basename $(pwd))" = "gitlab_ci" ]; then
        CI_FOLDER="."
else
    CI_FOLDER="gitlab_ci"
fi
INCLUDES_FILE="${CI_FOLDER}/includes.yml"

generate_includes() {
    echo "include:" > "$INCLUDES_FILE"

    find "$CI_FOLDER" -type f -name "*.yml" | while read -r file; do
        if [ "$file" != $INCLUDES_FILE ]; then
            echo "  - local: '$file'" >> "$INCLUDES_FILE"
        fi
    done

    echo "====== Generated ${INCLUDES_FILE} ======"
}

tracking() {
    if [ -z "$(which inotifywait)" ]; then
        echo "inotifywait not installed. Let's do it."
        sudo apt update
        sudo apt install -y inotify-tools
    fi

    if [ ! -f "$INCLUDES_FILE" ]; then
        echo "$INCLUDES_FILE doesn't exist."
        echo "====== Generating new includes.yml file ======"
        generate_includes
    fi

    # Watch for file changes using inotifywait
    while true; do
        echo "====== Tracking changes ======"
        inotifywait --quiet --recursive --monitor \
        --event modify,move,create,delete ./ \
        | if read changed; then
            echo "Change detected: $changed "
            echo "====== Generating new includes.yml file ======"
            generate_includes
        fi
    done
}

case $1 in
    generate_includes) "$@"; exit;;
esac

tracking