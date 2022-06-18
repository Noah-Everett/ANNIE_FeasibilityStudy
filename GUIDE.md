# **GUIDE - How To Reproduce ANNIE's Neutrino Argon Interaction Simulation Results**

## Table of Contents
- [setup](#setup)
- [make_geoms_1D.sh](#make_geoms_1Dsh)
- [GENIE Generator](#genie-generator)
    - [Make `.maxpl.xml` Files](#make-maxplxml-files)
    - [Generate Events](#generate-events)
- [g4annie_dirt_flux](#g4annie_dirt_flux)
- [WCsim](#wcsim)

## **setup**
```
source /annie/app/users/neverett/bin/setup
```

## **make_geoms_1D.sh**
```
$B/make_geoms_1D.sh --outDir=</pnfs/.../path/to/output> --material=argon --shape=tube --state=liquid --nFiles=5
```
Note, we will use the 5th file (`annie_v02_4.gdml`) as it has the largest container volume. For more information on each geometry, consult `make_geoms_1D.legend`.

PS: I know this script has more than 1 dimension of freedom; however, this refering to one spacial demension of freedom. While both radius and position change between geometries, they are independent parameters. There is an outdated `make_geoms_4D.sh` in `$B/depreciated` which allows for controle of x, y, z, and radius. Is there a better naming convension, yes.
 
## **GENIE Generator**

### **Make `.maxpl.xml` Files**
```
nohup $B/run_genie.sh -r=1 -n=0 -g=</path/to/geometries>/annie_v02_4.gdml -t=EXP_HALL_LV -f=* -m=+</path/to/geometries>/annie_v02_4.maxpl.xml -S=30000 --message-thresholds=$C/Messenger_warn.xml -o=</path/to/GENIE/output/dir> | tee </path/to/output/file.log or /dev/null if you dont want the file>
```
Uppon completion, feel free to delete all resulting files that aren't `annie_v02_4.maxpl.xml`.

### **Generate Events**
```
$B/run_genie_grid.sh -r=0 -n=1000 -g=</path/to/>annie_v02_4.gdml -t=TWATER_LV -f=* -m=</path/to/>annie_v02_4.maxpl.xml --message-thresholds=$C/Messenger_warn.xml -N=300
```

## **g4annie_dirt_flux**
```
nohup $B/run_g4dirt.sh -r=* -i=</path/to/genie/files/dir> -n=1000 -g=</path/to/>annie_v02_4.gdml -o=</path/to/output/dir> | tee </path/to/output/file.log or /dev/null if you dont want the file>
```

## **WCSim**
```
$B/run_wcsim_grid.sh -r=0 -p=</path/to/genie/and/annie_dirt_flux/files> -d=1000 -w=500 -g=</path/to/>annie_v02_4.gdml -o=</path/to/output/dir> -N=600
```