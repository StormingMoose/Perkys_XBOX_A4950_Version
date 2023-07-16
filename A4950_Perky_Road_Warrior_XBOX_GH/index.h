const char MAIN_page[] PROGMEM = R"=====(

<style>
  body {background-color: #FF0000; font-family: Times; font-size: 400% ; text-align: center; margin:1px auto; padding-top: 300px; width:auto; user-select: none;}
</style>
<body>     
 
  <h1 style="font-size:80%;">Perky Needs a Bluetooth Controller - None Connected --  press a button</h1>


<script type="text/javascript">

// global gamepad object
let gamepadIndex;
window.addEventListener('gamepadconnected', (event) => {
  gamepadIndex = event.gamepad.index;
});

  let paused = true;
  let shifted = false;
  let shiftedfast = false;
  let Forward = true;


setInterval(() => {

  if(gamepadIndex !== undefined) {
    // a gamepad is connected and has an index
   
    const myGamepad = navigator.getGamepads()[gamepadIndex];
    document.body.style.backgroundColor = "green" ;
    document.body.innerHTML = "  Perky Goes Wild!"; // reset page
 
    let y = myGamepad.axes[0];             // left joystick
    let x = myGamepad.axes[1];
    let z = myGamepad.axes[2];             //  right joystick
    let v = myGamepad.axes[3];
    
        
    if(y<.3 && y >-.3){steer(93); }   //  center dead spot for the steering and speed joystick
    else { if(y < -0.3){steer(93-((y+.3)*(28/.7)*-1)); }      //  joystick steer Left
           else{ if(y > 0.3){steer(93+(y-.3)*(28/.7));}   //  joystick right
               }
          }     

  
    if(x<.2 && x >-.2){speed(0); toggle("A");    }     //  center dead spot for the steering and speed joystick
    else { if(x < -0.2){speed((x+.2)*255/.8*-1);toggle("A"); }      //  joystick forward pressed
           else{ if(x > 0.2){speed((x-.2)*255/.8);toggle("B");}   //  joystick back pressed
               }
          }     
       
    if(z > 0.5){toggle("HR");}   // turn head lefter
    if(z < -0.5){toggle("HL");}  // turn head righter 
    if(v > 0.5){toggle("Up");}
    if(v < -0.5){toggle("Dn");}

              
    myGamepad.buttons.map(e => e.pressed).forEach((isPressed, buttonIndex) => { 
      if(isPressed) {
        
        if(buttonIndex == 17){ toggle("FO");}                   //  lights off and on
        if(buttonIndex == 8){ toggle("Flon");}
        if(buttonIndex == 3){ toggle("HC");}                     // center the head cameras      
        if(buttonIndex == 12){ toggle("CS");}                     // center the steering   

        if(buttonIndex == 0){ toggle("off");}                 // stop the motors
      //  if(buttonIndex == 5){ }                 //  
          

         
        
        // button is pressed; indicate this on the page.  Uncomment the next two lines, very handy for debugging  and identifying the buttons.

         //document.body.style.backgroundColor = "blue" ;
         //document.body.innerHTML += `<h1>Button ${buttonIndex} is pressed  </h1>`;
      
      }
    })
   
   
  }
  
}, 100) // print buttons that are pressed 10 times per second

   function toggle(m) {       // for sending single request
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + m, true);
     xhr.send();
   }

     function speed(howFast) {  // for sending two things SPEED trigger and howFast
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/SPEED?value="+ howFast , true);
     xhr.send();
   }

     function steer(pos) {       // for sending STEER triger and pos position
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/STEER?value="+ pos , true);
     xhr.send();
   }

</script>
</body>
</html>

 )====="; 

