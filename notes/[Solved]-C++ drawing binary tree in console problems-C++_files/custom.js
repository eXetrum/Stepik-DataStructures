function OnIResChange(){var itype=document.appsform.type.selectedIndex;var ilook=[1.72e-8,2.82e-8,1.43e-7,4.6e-7,2.44e-8,1.1e-6,6.99e-8,1.59e-8];var res='';if(itype>0){res=ilook[itype-1];res=res.toExponential();}
document.appsform.res.value=res;}
function OnResChange(){document.appsform.type.selectedIndex=0;}
function OnCalc(){var issel=document.appsform.ssel.selectedIndex;var ilsel=document.appsform.lsel.selectedIndex;var iphase=document.appsform.phase.selectedIndex;n=document.appsform.s.value;len=document.appsform.l.value;v=document.appsform.v.value;i=document.appsform.i.value;var res=document.appsform.res.value;var d;if(issel==0){if(n=="000000"||n=="6/0")n=-5;if(n=="00000"||n=="5/0")n=-4;if(n=="0000"||n=="4/0")n=-3;if(n=="000"||n=="3/0")n=-2;if(n=="00"||n=="2/0")n=-1;if(n>40){alert("AWG>40 is not valid");document.appsform.s.focus();document.appsform.s.select();return;}
d=0.127e-3*Math.pow(92,((36-n)/39));}
else if(issel==1){d=25.4*n/1000;}
else{d=n/1000;}
if(ilsel==0)
len*=0.3048;a=Math.PI*d*d/4.0;r=2*res*len/a;vd=i*r;if(iphase==2)vd*=(1.732/2);vdp=vd/v*100;document.appsform.vd.value=vd.toPrecision(6);document.appsform.vdp.value=vdp.toPrecision(6);document.appsform.r.value=r.toPrecision(6);}
function FinalResult99(){x=document.appsform.x.value;y=convert(x);y=roundresult(y);document.appsform.y.value=y;}
function FinalResult(){x=document.appsform.x.value;y=convert(x);y=roundresult(y);document.appsform.y.value=y;}
function FinalResult3test(){x=document.appsform.x.value;y=convert(x);y=roundresult1(y);document.appsform.y.value=y;}
function FinalResult3(){x1=document.appsform.x1.value;x2=document.appsform.x2.value;y=convert(x1,x2);y=roundresult(y);document.appsform.y.value=y;}
function FinalResult4(){x1=document.appsform.x1.value;x2=document.appsform.x2.value;x3=document.appsform.x3.value;y=convert(x1,x2,x3);y=roundresult(y);document.appsform.y.value=y;}
function FinalResult5(){x=document.appsform.x.value;y=convert1(x);y=roundresult(y);document.appsform.y1.value=y;y=convert2(x);y=roundresult(y);document.appsform.y2.value=y;}
function calcbase(b1,b2){x=document.appsform.x.value;document.appsform.y.value=convertbase(x,b1,b2);}
function calcbase2(){x=document.appsform.x.value;y=convert(x);document.appsform.y.value=y;}
function roundresult(x){y=parseFloat(x);y=roundnum(y,10);return y;}
function roundnum(x,p){var i;var n=parseFloat(x);var m=n.toPrecision(p+1);var y=String(m);i=y.indexOf('e');if(i==-1)i=y.length;j=y.indexOf('.');if(i>j&&j!=-1){while(i>0){if(y.charAt(--i)=='0')
y=removeAt(y,i);else
break;}
if(y.charAt(i)=='.')
y=removeAt(y,i);}
return y;}
function roundnum2(x,p){var i;var n=parseFloat(x);var m=n.toFixed(p);var y=String(m);i=y.length;j=y.indexOf('.');if(i>j&&j!=-1){while(i>0){if(y.charAt(--i)=='0')
y=removeAt(y,i);else
break;}
if(y.charAt(i)=='.')
y=removeAt(y,i);}
return y;}
function removeAt(s,i){s=s.substring(0,i)+s.substring(i+1,s.length);return s;}
var gcd=function(a,b){if(!b){return a;}
return gcd(b,a%b);};var c='$';function OnCountryChange(){clearresult();i=document.appsform.ddlcountry.selectedIndex;clook=['$','$','Kr','\u20AC','\u20AC','R','P','$','\u00A3','$',' '];dlook=[0,0,4,0,0,2,3,0,1,0,5];c=clook[i];document.appsform.ddlprice.selectedIndex=dlook[i];}
function OnapplianceChange(){clearresult();look=[600,75,3000,2400,1600,2000,70,90,2000,800,100,40,50,200,1500,200,70,1000,1600,2000,4000,50];i=document.appsform.ddlappliance.selectedIndex;if(i==0)return;document.appsform.Text1.value=look[i-1];document.appsform.ddlPowercs.selectedIndex=0;}
function CalCulateConsumption(){i=document.appsform.ddlcountry.selectedIndex;clook=['$','$','Kr','\u20AC','\u20AC','R','P','$','\u00A3','$',' '];c=clook[i];i1=document.appsform.ddlcountry.selectedIndex;i2=document.appsform.ddlPowercs.selectedIndex;i3=document.appsform.ddlprice.selectedIndex;p=document.appsform.Text1.value;h=document.appsform.Text2.value;kwh_cost=document.appsform.Text3.value;if(i2==0)p/=1000;kwh=p*h;if(i3>1)kwh_cost*=100;day_cost=kwh*kwh_cost/100;month_cost=day_cost*30.4375;year_cost=day_cost*365.25;day_cost=roundnum(day_cost,5);month_cost=roundnum(month_cost,5);year_cost=roundnum(year_cost,5);kwh_day=p*h;kwh_month=kwh_day*30;kwh_year=kwh_day*365;kwh_day=roundnum(kwh_day,5);kwh_month=roundnum(kwh_month,5);kwh_year=roundnum(kwh_year,5);document.getElementById("txtperday").value=kwh_day;document.getElementById("txtpermonth").value=kwh_month;document.getElementById("txtperyear").value=kwh_year;var pp=c+day_cost;document.getElementById("Text5").value=c+day_cost;document.getElementById("Text6").value=c+month_cost;document.getElementById("Text7").value=c+year_cost;}
function OnReset(){c='$';}
function clearresult(){document.appsform.Text5.value="";document.appsform.Text6.value="";document.appsform.Text7.value="";document.getElementById("txtperday").value="";document.getElementById("txtpermonth").value="";document.getElementById("txtperyear").value="";;}