#!/bin/bash
echo converting course data to JSON...

gcc -o generate_json ./c-files/generate-JSON.c
gcc -o remove_invalid ./c-files/remove-invalid-JSON.c

COURSES="./data/courses.js"
COURSES_UNFINALIZED="./data/courses-unfinalized.js"
DIRECTORY_OF_CLASSES="./data/directory-of-classes.txt"

if [ -e $COURSES ]
then
    rm $COURSES
fi

if [ -e $COURSES_UNFINALIZED ]
then
    rm $COURSES_UNFINALIZED
fi
#not including DENT, MEDI, RADG, RADI, RADY, SURY due to seg faults
for dept in AAAD AERO AHEC AHSC AMST ANES ANTH APPL APSM ANES ANTH APPL APSM ARAB ARCH ARMY ARTH ARTS ASHV ASIA ASTR \
            BBSP BFRS BIOC BIOL BIOS BMEJ BMEU BMME BUSA BUSG BUSI BUSS CAPS CBAM CBIO CBMC CBPH CHEM CHER CHIN CLAR \
            CLAS CLIC CLSC CMPL COMM COMP CORE CRMH CZCH DENG DERM DHED DHYG DNDG DPET DPMP DPOP DRAM DTCH ECON EDMX \
            EDUC ENDO ENEC ENGL ENVR EPID ERMD EURO EXSS FMME FOLK FREN GEOG GEOL GERJ GERM GLBE GLBL GNET GOVT GRAD \
            GREK GSLL HBEH HEBR HIST HMSC HNRS HNUR ICMU IDST IHMS IIOC INSL ITAL JAPN JWST LATN LAW  LFIT LFXP LGLA \
            LING LTAM LVE  MAC  MACF MASC MATH MBA  MCRO MEDF MEJO MESE MHCH MNGT MOPH MTEC MTSC MUSC NAVS NBIO NCCU \
            NDSS NEUR NCSI NURS NUSJ NUTR OBGN OBIO OCBM OCCT OCSC OMED OPER OPHT ORAD ORPA ORTH ORTS OTOL PACE PASC \
            PATH PATY PEDO PEDS PERI PERU PHCO PHCY PHIL PHNU PHRS PHYA PHYI PHYS PHYT PHYY PLAN PLCY PLSH PMED POLI \
            PORT PROS PROS PRSN PSYC PSYY PUBA PUBH PWAD RECR RELI RLGE ROBT ROML RUSS SECR SLAV SOCI SOCM SOWO SPAN \
            SPCL SPHG SPHS STOR SURS SWAH TOXC TREQ TURK WGST WMST WOLO

do
    #echo $dept
    cat $DIRECTORY_OF_CLASSES | grep -A 5 "$dept       [0-7]" |./generate_json $dept >> $COURSES_UNFINALIZED
done

echo var data = \`[ >> $COURSES
cat $COURSES_UNFINALIZED | ./remove_invalid >> $COURSES
echo {}]\` >> $COURSES #end of list; included empty object "{}" to deal with the dangling comma at the end of the prev. item in list

#clean up
rm $COURSES_UNFINALIZED
rm generate_json 
rm remove_invalid

echo done! 
echo the data is stored as the following location: data/courses.js
