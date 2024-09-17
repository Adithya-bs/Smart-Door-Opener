
const socket = io();

const but = document.getElementById('b1');
const message = document.getElementById('message');
// const bod = document.getElementByTagName('body');

// bod.addEventListener('load', (e) => 

window.onload = fetchData;
function fetchData(){
    axios.get('/door_status')
        .then(response => {
          // Handle the response
          const data = response.data.data;
          // Display the received data on the page
        //   message.innerHTML = 'Door status : ' + content;
            
          console.log(data);
          if (data == 0) {
              but.style.backgroundColor = "#94F3E4";
              message.innerHTML = 'Door status : closed';
          }
          else {
            //   but.style.backgroundColor = "white";
              but.style.backgroundColor = "#37AA9C";
              message.innerHTML = 'Door status : opened';

          }
    
        })
        .catch(error => {
          console.error('Error fetching data:');
        });

};

but.addEventListener('click', (e) => {

    //socket.emit('clicked', 'button was clicked.');
    if (message.innerHTML.endsWith("opened")) {
        socket.emit('user_command', 'off');
    }
    else {
        socket.emit('user_command', 'on');
    }
    // console.log("button was clicked");
});

// socket.on('message', (msg) => {
//     const content = msg;
//     message.innerHTML = content;

// });
socket.on('door', (msg) => {
    const content = msg;
    message.innerHTML = 'Door status : ' + content;

    //console.log(msg);
    if (msg === "closed") {
        but.style.backgroundColor = "#94F3E4";
    }
    else {
        but.style.backgroundColor = "#37AA9C";
    }
});

