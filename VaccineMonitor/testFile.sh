#!/bin/bash

if [ $# -eq 4 ];
then
    virusf=$1 
    countriesf=$2
    numlines=$3
    dupAllowed=$4
else
    echo "Wrong input.\n"
    exit 0
fi


#array of first names
fnames=('Maria' 'John' 'Frankie' 'Jane' 'George' 'Selven' 'Kate' 'Christina' 'Nick' 'Wilson' 'James' 'Alyssa'
'Ellie' 'David' 'Annie' 'Tomas' 'Byron' 'Mao' 'Julian' 'Alex' 'Ni_Hao' 'Paul' 'Rebecca' 'Newt' 'Albus' 'Eva')


#array of last names
lnames=('Johnson' 'Kit' 'Ma' 'Scamander' 'Lestrange' 'Malfoy' 'Vonaparte' 'Clark' 'Kolstaad' 'McCourtney'
'Green' 'Burton' 'Parker' 'Brown' 'Turner' 'Dumbledore' 'Sanchez' 'Smith' 'House' 'Link' 'Jackson' 'Newbie'
'Yang' 'Bailey' 'Scott')


#reads virus txt and puts every line in an array of viruses
file=$virusf
vfcount=0
viruses=()
while read line; do
    
    length=${#line}
    viruses[vfcount]=${line:0:len-1}
    ((vfcount++))
done < $file


#reads countries txt and puts every line in an array of countries
file=$countriesf
cfcount=0
countries=()
while read line; do
    
    length=${#line}
    countries[cfcount]=${line:0:len-1}
    ((cfcount++))
done < $file


#array of IDs (from 0 to 9999)
IDs=()
for (( counter=0; counter<=9999; counter++ ))
do
    IDs[counter]=$counter
done


#shuffle the elements of array of IDs
for (( counter=0; counter<=9999; counter++ ))
do
    temp1=$(( $RANDOM % 9999 + 0 ))
    temp2=${IDs[counter]}

    IDs[counter]=${IDs[temp1]}
    IDs[temp1]=$temp2
done


#the input file we create
touch TestinputFile.txt

count=0
index=-1
while [ $count -ne $numlines ]
do 
    #choose a random country from array
    country=$(( $RANDOM % cfcount ))
    country=${countries[country]}

    #choose a random virus from array
    virus=$(( $RANDOM % vfcount ))
    virus=${viruses[virus]}


    yesno=$(( $RANDOM % 2 ))
    #pick a random number between 0-9999 from array (ID)
    #if $4=0 duplicate IDs not allowed
    if [[ ($4 -eq 0 || $yesno -eq 0 || $count -eq 0) ]];
    then
        ((index++))
        ID=${IDs[index]}

    #else duplicate IDs allowed
    else
        ID=${IDs[index]}
    fi


    #pick a random number between 1-120 (age)
    age=$(( $RANDOM % 120 + 1 ))

    #pick a random number between 0-1 (yes or no)
    yesno=$(( $RANDOM % 2 ))
    if [ $yesno -eq 1 ];
    then
        yn="YES"
    else
        yn="NO"
    fi

    #choose a random first name from array
    first_name=$(( $RANDOM % 25 ))
    first_name=${fnames[first_name]}

    #choose a random last name from array
    last_name=$(( $RANDOM % 25 ))
    last_name=${lnames[last_name]}

    #pick a random number between 0-1 (date included or not)
    yesno=$(( $RANDOM % 2 ))


    #if yesno=1 date included
    if [ $yesno -eq 1 ];
    then
        #pick a random number between 1-30 (day)
        day=$(( $RANDOM % 30 + 1 ))

        #pick a random number between 1-12 (month)
        month=$(( $RANDOM % 12 + 1 ))

        #pick a random number between 2000-2020 (year)
        year=$(( $RANDOM % 20 + 2000 ))

        #we merge the date
        date="$day-$month-$year"
    
    #else date not included
    else
        date=""
    fi


    #we merge the record
    record="$ID $first_name $last_name $country $age $virus $yn $date"

    #we put the record in the file
    echo $record>>TestinputFile.txt

    ((count++))
done