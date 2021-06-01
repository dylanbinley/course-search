var rmpData, courses;

function onStart(data){
    courses = JSON.parse(data);
    rmpData = [];
    const TOTAL_PAGES = 132;
    for (var pageNum = 1; pageNum < TOTAL_PAGES; pageNum++){
        var text = 'https://www.ratemyprofessors.com/find/professor/?department=&institution=The+University+of+North+Carolina+at+Chapel+Hill&page=' + pageNum + '&query=*%3A*&queryoption=TEACHER&queryBy=schoolId&sid=1232&sortBy='
        $.getJSON(text, function(data){
            var professors = data.professors;
            for (var i = 0; i< professors.length; i++){
                rmpData.push(professors[i]);
            }
            searchCourses()
        });
    }
}

function searchCourses(){
    let selectedCourses = []
    var parsedInput = splitInput(document.getElementById("search").value); 
    if (parsedInput.length === 0){ //empty search bar
        generateTable(null);
        return;
    }
    for (var i = 0; i < courses.length; i++){
        if (courses[i].section < 50 //exclude recitations 
            && courses[i].subject.toLowerCase() == parsedInput[0].toLowerCase() 
            && courses[i].number >= parsedInput[1] 
            && courses[i].number <= parsedInput[2]){
            selectedCourses.push(courses[i]);
        }
    }
    selectedCourses.sort(function(course1,course2){
        return course1.number - course2.number
    })
    generateTable(selectedCourses);
}

function generateTable(selectedCourses){
    let table= document.querySelector("table");
    table.innerHTML = ` <thead>
                        <tr>
                            <th>Course Number</th>
                            <th>Course Name</th>
                            <th>Time</th>
                            <th>Professor</th>
                            <th>Rating</th>
                            <th>Status</th>
                        </tr>
                   </thead>
                 `;
    if (selectedCourses === null){
        return;
    }
    selectedCourses.forEach(function(course){
        let courseNumber= document.createTextNode(course.subject + " " + course.number + " " + course.section);
        let courseName = document.createTextNode(course.name);
        let time = course.days === "TBA" ? document.createTextNode("TBA"): document.createTextNode(course.days + ' ' + course.time);
        let professor = getProfessorObj(course)
        let classStatus= getClassStatus(course)
        /*if (classStatus.alt === "closed" || classStatus.alt === "waitlist"){
            console.log("skip");
            return;
        }
        */
        let row = table.insertRow();
        row.insertCell().appendChild(courseNumber);
        row.insertCell().appendChild(courseName);
        row.insertCell().appendChild(time);
        row.insertCell().appendChild(professor.name);
        row.insertCell().appendChild(professor.rating);
        row.insertCell().appendChild(classStatus);
    });

}

function splitInput(input){
    var split = input.split(/(?:-| )+/) //split on space/dash
    split = split.filter(function(value, index, arr){ 
        return value !== ""; //remove duplicate spaces/dashes
    });
    if (split.length == 1){ //search entire department (ex. COMP) 
        split[1] = 0;
        split[2] = 999;
    }
    else if (split.length == 2){ //search specific class (ex. COMP 211)
        split[2] = split[1];
    }
    return split;
}

function getProfessorObj(course){
    let profName = document.createTextNode(course.tFname + " " + course.tLname);
    let info = getRateMyProfessorsInfo(course); 
    if (info != null){
        let link = document.createElement('a');
        link.href = 'https://www.ratemyprofessors.com/ShowRatings.jsp?tid=' + info.id
        link.appendChild(profName);
        return { name : link, rating: createRating(info.rating) }
    }
    return { name: profName, rating: createRating("N/A") }
}

function getRateMyProfessorsInfo(course){
    for (var j = 0; j< rmpData.length; j++){
        if (course.tLname.toLowerCase() == rmpData[j].tLname.toLowerCase() 
            && course.tFname.toLowerCase() == rmpData[j].tFname.toLowerCase()){
            return {id: rmpData[j].tid, rating: rmpData[j].overall_rating}
        }
    }
    return null;
}

function getClassStatus(course){
    var class_status;
    var enrollment = course.enrollment;
    var capacity = course.capacity;
    var availability_number = 0; //tracks number of open seats or waitlist total
    if (enrollment == "N/A" || capacity == "N/A"){
        class_status = "unknown";
    }
    else if (enrollment < capacity){
        class_status = "open"
        availability_number = capacity - enrollment;
    }
    else {
        var waitlist_total = course.waitlist_total;
        var waitlist_capacity = course.waitlist_capacity;
        if (waitlist_total == "N/A" || waitlist_capacity == "N/A" || waitlist_capacity <= waitlist_total ){
            class_status = "closed";
        }
        else {
            class_status= "waitlist";
            availability_number = waitlist_total;
        }
    }
    return createImage(class_status, availability_number);
}

function createImage(type, availability_number){
    var img, src; 
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
    img = document.createElement("img");
    img.setAttribute("src", src); 
    img.setAttribute("width", "20");
    img.setAttribute("height", "20");
    img.setAttribute("alt", type); 
    if (type == "waitlist"){
        img.setAttribute("title", "Waitlist total: " + availability_number); 
    }
    else{
        img.setAttribute("title", "Available seats: " + availability_number); 
    }
    return img 
}

function createRating(rating){
    var div, color;
    switch(Math.ceil(rating-.5)){
        case 5: //5-4.5
            color = "green"
            break;
        case 4: //4.49-3.5
            color = "rgb(0,230,0)";
            break;
        case 3: //3.49-2.5
            color = "orange"
            break;
        case 2: //2.49-1.5
            color = "rgb(255,0, 0)";
            break;
        case 1: //<1.5
        case 0:
            color = "rgb(200,0, 0)";
            break;
        default: //"N/A"
            color = "gray"
            break;
    }
    div = document.createElement("div");
    div.innerHTML = rating 
    div.style.color ="white"
    div.style.backgroundColor = color
    div.style.width="35px"
    div.style.paddingTop = "5px"
    div.style.paddingBottom = "5px"
    div.style.marginRight = "auto"
    div.style.marginLeft= "auto"
    div.style.fontWeight = "bold"
    return div;
}
