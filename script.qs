//module.log("Start script");
var cnt_gled_flash = 0;
var cnt_click_duration = 0;
var flip_flop = false;
var click = false;
var cntS=0;
var cntD=0;
var a1=[];
var b1=[6,7,2,1];
var j;


function onEmgData(emg1, emg2, emg3, emg4, emg5, emg6, emg7, emg8) {   //Fired in about 1000Hz
//module.setOut(1,8);
if(++cntD==5)//7
{
cntD=0;

module.addByte(255);
module.addByte(emg1);
module.addByte(emg2);
//module.addInt(emg3);
//module.addInt(emg8);
if(++cntS==2)
{
cntS=0;
//module.log("MAX_LOG: "+emg1);
module.sendUdp();
module.clearBuff();     //Clear UDP buffer
}
}
}


/*
function onEmgData(emg1, emg2, emg3, emg4, emg5, emg6, emg7, emg8) {

//module.setOut(7,8);
module.clearBuff();
module.addInt(emg1);
module.addInt(emg2);
//module.addInt(emg3);
//module.addInt(emg4);
//module.addInt(emg5);
//module.addInt(emg6);
//module.addInt(emg7);
//module.addInt(emg8);
//module.sendUdp();

}
*/


/*
function onGyroData(gyro_x, gyro_y, gyro_z) {    //Fired in about 170Hz

if (Math.abs(gyro_x) > 2000 && !click) {
module.setOut(6, 10);      //Turn-on switch
cnt2 = 0;
click = true;
}
if (click && cnt_click_duration++ > 20) {
module.setOut(6, 0);       //Turn-off switch
click = false;
}

cntM++;
if(cntM==2)
{cntM=0;
module.clearBuff();     //Clear UDP buffer
module.addInt(gyro_x);  //Add data to the buffer
module.sendUdp();       //Send buffer
}
}
*/

/*
function onUdpData() {

if (module.getDataSize()==1) {
module.log("dataSize "+ module.getDataSize());
module.log("getByte "+module.getByte());
}
}
*/



/*
function onUdpData() {

module.log("UDP received "+ module.getDataSize());          //Log UDP receive event
while (module.getDataSize() > 1) {
module.setOut(var a1=module.getByte(),
var a2=module.getByte());   //Manipulate a switch depending on received data
module.setOut(a1,a2);
}
}
*/


///////////////////////////////////////
function onUdpData() {
//module.setOut(8,8);
var DS = module.getDataSize();
for (i=0;i<DS;i++)
{
j=i%4;
a1[j]=module.getByte();
module.setOut(b1[j],a1[j]);

}
//module.log(DS);
//module.log(a1[0]+" "+a1[1]);
}
