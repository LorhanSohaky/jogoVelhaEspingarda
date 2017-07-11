function setXY(x,y) {
    document.getElementById("x").value=x;
    document.getElementById("y").value=y;
    document.getElementById("formulario").submit();
 }


var getUrlParameter = function getUrlParameter(sParam) {
     var sPageURL = decodeURIComponent(window.location.search.substring(1)),
         sURLVariables = sPageURL.split('&'),
         sParameterName,
         i;

     for (i = 0; i < sURLVariables.length; i++) {
         sParameterName = sURLVariables[i].split('=');

         if (sParameterName[0] === sParam) {
             return sParameterName[1] === undefined ? true : sParameterName[1];
         }
     }
 };

function getModo(){

     if(document.getElementById("modo").value=="0"){
 	       document.getElementById("modo").value=getUrlParameter('modo');
           /*modo=prompt("Modo:\n[1] - PVP\n[2] - PC",1);
           document.getElementById("modo").value=modo;*/
       }
}


window.onload=getModo();
