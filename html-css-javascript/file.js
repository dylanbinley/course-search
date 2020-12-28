var rmpdata = [];
var mydata= [];
var professorlink = 'https://www.ratemyprofessors.com/ShowRatings.jsp?tid='
function onStart(data){
    mydata = JSON.parse(data);
    for (var pageNum = 1; pageNum < 132; pageNum++){ 
        var text = 'https://www.ratemyprofessors.com/find/professor/?department=&institution=The+University+of+North+Carolina+at+Chapel+Hill&page=' + pageNum + '&query=*%3A*&queryoption=TEACHER&queryBy=schoolId&sid=1232&sortBy='
        $.getJSON(text, function(data){
            var professors = data.professors;
            for (var i = 0; i< professors.length; i++){
                rmpdata.push(professors[i]);
            }
            loadLocalClasses()
        });
    }
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
    tble.innerHTML = ` <thead>
                <tr>
                    <th>Course Number</th>
                    <th>Course Name</th>
                    <th>Time</th>
                    <th>Professor</th>
                    <th>Rating</th>
                    <th>Status</th>
                </tr>
            </thead>`;
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
                    cell.appendChild(createRating(rmpdata[j].overall_rating));
                    hasRating = true;
                }

            }
            if (!hasRating){
                cell.appendChild(txt);
                let cell3 = row.insertCell();
                cell3.appendChild(createRating("N/A"));
            }

            cell = row.insertCell();
            var enrollment = mydata[i].enrollment;
            var capacity = mydata[i].capacity;
            var class_status;
            if (enrollment == "N/A" || capacity == "N/A"){
                class_status = "unknown";
            }
            else if (enrollment < capacity){
                class_status = "open"
            }
            else {
                var waitlist_total = mydata[i].waitlist_total;
                var waitlist_capacity = mydata[i].waitlist_capacity;
                if (waitlist_total == "N/A" || waitlist_capacity == "N/A"){
                    class_status = "closed";
                }
                else if (waitlist_capacity > waitlist_total){
                    class_status= "waitlist";
                }
                else{
                    class_status = "closed";
                }
            }
            cell.appendChild(createImage(class_status));
        }
    }
}
function createImage(type){
    var x = document.createElement("img");
    var src; 
    switch(type) {
        case "open":
            src = "https://upload.wikimedia.org/wikipedia/commons/thumb/0/0e/Ski_trail_rating_symbol-green_circle.svg/1024px-Ski_trail_rating_symbol-green_circle.svg.png"
            break;
        case "waitlist":
            src = "https://upload.wikimedia.org/wikipedia/commons/thumb/b/b5/Yellow_triangle.svg/144px-Yellow_triangle.svg.png"
            break;
        default:
            src = "https://upload.wikimedia.org/wikipedia/commons/thumb/0/0d/Ski_trail_rating_symbol-blue_square.svg/900px-Ski_trail_rating_symbol-blue_square.svg.png"
    } 
    x.setAttribute("src", src); 
    x.setAttribute("width", "20");
    x.setAttribute("height", "20");
    x.setAttribute("title", type); 
    x.setAttribute("alt", type); 
    return x
}
function createRating(rating){
    if (rating == "N/A"){
        color = "gray"
    }
    else if (rating >= 4.5){
        color = "green"
    }
    else if(rating >= 3.5){
        color = "rgb(0,230,0)";
    }
    else if (rating >= 2.5){
        color = "orange"

    }
    else if (rating >= 1.5){
        color = "rgb(255,0, 0)";
    }

    else {
        color = "rgb(200,0, 0)";
    }
    txt= document.createElement("p");
    txt.innerHTML = rating 
    txt.style.color ="white"
    txt.style.backgroundColor = color
    txt.style.width="35px"
    txt.style.paddingTop = "5px"
    txt.style.paddingBottom = "5px"
    txt.style.marginRight = "auto"
    txt.style.marginLeft= "auto"
    txt.style.fontWeight = "bold"
    return txt;
}
function logText(){
    var input = document.getElementById("search").value;
    console.log(input);
}
