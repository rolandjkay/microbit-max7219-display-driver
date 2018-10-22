#!/bin/sh

cp build/$(yt --plain target | head -n 1 | cut -f 1 -d' ')/test/$(yt --plain ls | head -n 1 | cut -f 1 -d' ')-test-test-card-combined.hex  /Volumes/"MICROBIT"
