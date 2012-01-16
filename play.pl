#!/usr/bin/perl -w

`make`;
`./update_image.sh`;
`./run_bochs.sh &`;

