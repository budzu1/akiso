#!/bin/bash

get_cat_image() {
    cat_url=$(curl -s 'https://api.thecatapi.com/v1/images/search' | jq -r '.[0].url')
    wget -O cat_image.jpg "$cat_url"  # Download the cat image
    catimg cat_image.jpg  # Display the cat image using catimg
    rm cat_image.jpg  # Clean up the downloaded image
}


get_chuck_norris_quote() {
    chuck_quote=$(curl -s 'https://api.chucknorris.io/jokes/random' | jq -r '.value')
    echo "Chuck Norris: $chuck_quote"
    echo 
}

get_cat_image
get_chuck_norris_quote