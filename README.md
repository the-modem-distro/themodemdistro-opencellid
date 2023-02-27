# The Modem Distro: OpenCellid database web helper
This repo contains the tools and scripts used to generate carrier specific binary files from the OpenCellid Database and publish them in a web server. You can also repurpose it to create the files locally and push them as needed.

## Included in this repo:
- Script to trigger the download of the database file from OpenCellid ([API key required!](https://opencellid.org/register.php))
- Small C tool to extract the contents of the csv and create the files for each carrier
- Sample NGINX configuration file

## Instructions
1. Build `ocidconv` with `make`
2. Edit `sync_ocid` and set the API key you obtained from OpenCellid, and change the paths to the ones you want to use. For this example we will use `/opt/opencellid` as the place where `sync_ocid` and `ocidconv` are located, and `/var/www/opencellid` as the web server root path
3. Create the web server path
4. Use the nginx example configuration file included or create your own, pointing at the web server path you specified in the script, so you can expose the files.
4. Do a test run of the `sync_ocid` script. It should download `cell_towers.csv.gz` in `/opt/opencellid/tmp`, extract it, and create the output files in `/opt/opencellid/tmp`. After it finished, it should move all the files to the web server path.
5. If everything was correctly set up, navigating to your web server should show you the directory with all the files. You can now run a script from your phone to retrieve the files. You can find an example [here](https://github.com/the-modem-distro/pinephone_modem_sdk/tree/kirkstone/tools/helpers)
6. Use crontab to run the script every once in a while (OpenCellid allows you to download the same file twice per day on the free tier)

## Open Cellid License
[OpenCelliD Project](https://opencellid.org/) is licensed under a [Creative Commons Attribution-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-sa/4.0/)

## Thank you
    - OpenCellid, for all the data that makes this possible
    - @marcin, for the code to repack the cell towers database file
    - @codephase, for the example script to retrieve the cell data and push it to the modem