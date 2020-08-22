var rmpdata = [];
var mydata= [];
var professorlink = 'https://www.ratemyprofessors.com/ShowRatings.jsp?tid='
function logData(){
    console.log(rmpdata);
}
function loadData(data){
    mydata = JSON.parse(data);
    for (var pageNum = 1; pageNum < 132; pageNum++){ 
        var text = 'https://www.ratemyprofessors.com/find/professor/?department=&institution=The+University+of+North+Carolina+at+Chapel+Hill&page=' + pageNum + '&query=*%3A*&queryoption=TEACHER&queryBy=schoolId&sid=1232&sortBy='
        $.getJSON(text, function(data){
            var professors = data.professors;
            for (var i = 0; i< professors.length; i++){
                rmpdata.push(professors[i]);
            }
        });
    }
    var searchbar = document.getElementById("search");
    searchbar.addEventListener("keydown", function (e) {
        if (e.keyCode === 13) {  //checks whether the pressed key is "Enter"
            loadLocalClasses();
        }
    });
    logData();
}
function loadLocalClasses(){
    var input = document.getElementById("search").value;
    var parsedInput = input.split(/(?:-| )+/);
    if (parsedInput.length == 1){
        parsedInput[1] = 0;
        parsedInput[2] = 999;
    }
    else if (parsedInput.length == 3 && parsedInput[2] > 999)
    {
        parsedInput[2] = 999;
    }
    else if (parsedInput.length == 2){
        parsedInput[2] = parsedInput[1];
    }
    let tble = document.querySelector("table");
    tble.innerHTML = "";
    for (var i = 0; i< mydata.length; i++){
        if (mydata[i].time != "TBA" && mydata[i].section < 100 && mydata[i].subject.toLowerCase() == parsedInput[0].toLowerCase() && mydata[i].number >= parsedInput[1] && mydata[i].number <= parsedInput[2]){
            let row = tble.insertRow();
            let cell = row.insertCell();
            let txt = document.createTextNode(mydata[i].subject + " " + mydata[i].number + " " + mydata[i].section);
            cell.appendChild(txt);

            cell = row.insertCell();
            txt = document.createTextNode(mydata[i].name);
            cell.appendChild(txt);

            cell = row.insertCell();
            txt = document.createTextNode(mydata[i].days + ' ' + mydata[i].time);
            cell.appendChild(txt);

            cell = row.insertCell();
            let a = document.createElement('a');
            txt = document.createTextNode(mydata[i].tFname + " " + mydata[i].tLname);
            a.appendChild(txt);
            a.title = txt;

            let hasRating = false;
            for (var j = 0; j< rmpdata.length; j++){
                if (mydata[i].tLname.toLowerCase() == rmpdata[j].tLname.toLowerCase() && mydata[i].tFname == rmpdata[j].tFname){
                    a.href = 'https://www.ratemyprofessors.com/ShowRatings.jsp?tid=' + rmpdata[j].tid
                    cell.appendChild(a);
                    cell= row.insertCell();
                    txt= document.createTextNode(rmpdata[j].overall_rating);
                    cell.appendChild(txt);
                    hasRating = true;
                    break;
                }

            }
            if (!hasRating){
                cell.appendChild(txt);
                let cell3 = row.insertCell();
                let txttt = document.createTextNode("N/A");
                cell3.appendChild(txttt);
            }

            cell = row.insertCell();
            var enrollment = mydata[i].enrollment;
            var capacity = mydata[i].capacity;
            var str;
            if (enrollment == "N/A" || capacity == "N/A"){
                str = "Unkown";
            }
            else if (enrollment < capacity){
                str = "open";
            }
            else {
                var waitlist_total = mydata[i].waitlist_total;
                var waitlist_capacity = mydata[i].waitlist_capacity;
                if (waitlist_total == "N/A" || waitlist_capacity == "N/A"){
                    txt = "closed";
                }
                else if (waitlist_capacity > waitlist_total){
                    str = "waitlist";
                }
                else{
                    str = "closed";
                }
            }
            txt = document.createTextNode(str);
            cell.appendChild(txt);
        }
    }
}
function logText(){
    var input = document.getElementById("search").value;
    console.log(input);
}
