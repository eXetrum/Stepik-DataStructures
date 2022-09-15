/* Copyright (c) 2008-2022, Quantcast Corp. https://www.quantcast.com/legal/license */
!function(window){function RequireDependencyError(e){Error.apply(this),this.name="RequireDependencyError",this.message=e||""}RequireDependencyError.prototype=Error.prototype;var amd={},definitions={};amd.require=function(e,t){"function"==typeof e&&(t=e,e=[]);for(var n=[],r=0;r<e.length;r++){var o=e[r];if(!Object.prototype.hasOwnProperty.call(definitions,o))throw new RequireDependencyError("No module named "+o+" has been defined");n[r]=definitions[o]}return t.apply({},n)};var array=Array.prototype,available=function(e,t){return"function"==typeof e[t]},map="map",forEach="forEach",reduce="reduce",indexOf="indexOf";with(available(array,"map")||(array.map=function(e,t){var n=[];t||(t=this);for(var r=0;r<this.length;r++)n[r]=e.call(t,this[r],r,this);return n}),available(array,"forEach")||(array.forEach=array.map),available(array,"reduce")||(array.reduce=function(e,t){var n=0;for(void 0===t&&(t=this[n++]);n<this.length;n++)t=e.call(this,t,this[n],n,this);return t}),available(array,"indexOf")||(array.indexOf=function(e){for(var t=0;t<this.length;t++)if(this[t]==e)return t;return-1}),amd.define=function(e,t,n){Object.prototype.hasOwnProperty.call(definitions,e)||(definitions[e]=amd.require(t,n))},amd)define("quant/dateutils",[],function(){var e=function(){return(new Date).getTime()},t=function(){return new Date},n=function(e){var t=new Date(2e3,e,1,0,0,0,0),n=t.toGMTString();return t-new Date(n.substring(0,n.lastIndexOf(" ")-1))};return{now:t,nowTimestamp:e,isDst:function(){return n(0)!==n(6)?1:0},tzOffset:n}}),define("quant/origin",["quant/dateutils"],function(e){return function(t){for(var n=t.domain||"",r=new Date(0).toUTCString(),o=new Date(e.nowTimestamp()+864e5).toUTCString(),a=n.split("."),i="",u=2;u<=a.length;u++){i=a.slice(-u).join(".");var c="_dlt="+"1; path=/; domain="+i+"; expires="+o;if(t.cookie=c,/_dlt=1\b/.test(t.cookie))return t.cookie="_dlt="+"; path=/; domain="+i+"; expires="+r,i}return t.cookie="_dlt="+"; path=/; domain="+i+"; expires="+r,n}}),define("quant/windows",[],function(){return function(e,t){if(void 0===e)throw new Error("window many not be undefined");if(void 0===t)throw new Error("top may not be undefined");t=t.self,this.depth=0;var n=e.self;for(this.top=n;n!==t;){n=n.parent.self;try{n.location.href&&(this.url=n.location.href,this.top=n)}catch(e){}this.depth++}this.locate=function(n){for(var r=e;;){try{if(n in r.frames)return r}catch(e){}if(r===t)break;r=r.parent.self}}}}),define("quant/log",["quant/dateutils"],function(e){function t(){this.isDebug=/qcdbgc=1$/.test(window.location.toString());var t=function(){return e.now().toString()},n=function(e,n){"undefined"!=typeof console&&console.log.apply(console,[e+" "+t()].concat([].slice.call(n)))};this.error=function(){n("ERROR",arguments)},this.debug=function(){this.isDebug&&n("DEBUG",arguments)}}return t}),define("quant/ready",[],function(){function e(){var e=!1,t=[];document.readyState in{complete:!0,interactive:!0}&&(e=!0);var n=function(){for(e=!0;t.length>0;)t.shift()()};document.addEventListener?(document.addEventListener("DOMContentLoaded",n,!1),window.addEventListener("load",n,!1)):document.attachEvent&&(document.attachEvent("onreadystatechange",n,!1),window.attachEvent("onload",n)),this.ready=function(n){e?n():t.push(n)}}return(new e).ready}),define("quant/promise",[],function(){function e(u){var c,s,f=[],l=t,p=function(e){return e},d=function(e,t,n,r){try{var a=e(r);o(a)?(a.then(t),a.catch(n)):t(a)}catch(e){n(e)}},h=function(e){c=e,l=n,f.forEach(function(t){t.push(e),d.apply(0,t)})},v=function(e){s=e,l=r,f.forEach(function(t){t[r](e)})},g=function(e,t,n){return function(r){d(e,t,n,r)}};try{u(h,v)}catch(e){v(e)}return{then:function(o){switch(l){case t:return new e(function(e,t){f.push([o,e,t])});case n:return a(o(c));case r:return i(s)}},catch:function(o){switch(l){case t:return new e(function(e,t){f.push([p,e,g(o,e,t)])});case n:return a(c);case r:return a(o(s))}}}}var t=0,n=1,r=2,o=function(e){return"object"==typeof e&&"then"in e&&"function"==typeof e.then},a=function(e){return o(e)?e:{then:function(t){return a(t(e))},catch:function(){return this}}},i=function(e){return{then:function(){return this},catch:function(t){return a(t(e))}}};return e.resolve=a,e.reject=i,e.all=function(e){return e.length?e.map(function(e){return e.then(function(e){return[e]})}).reduce(function(e,t){return e.then(function(e){return t.then(function(t){return e.concat(t)})})}):a([])},e}),define("quant/loader",["quant/ready","quant/promise"],function(e,t){function n(n,r){var o,a=[];e(function(){o=r.getElementsByTagName("head")[0]});var i=function(e){var t=new Image;return t.src=e,t};this.image=function(e){return new t(function(t,n){var r=i(e);a.push(r),r.onload=function(){t(r),a.shift(),delete r.onload,delete r.onerror},r.onerror=n})},this.beacon=function(e){var t=n.navigator;t&&t.sendBeacon?t.sendBeacon(e):i(e)};var u=function(e,t,n){var o=r.createElement("script");o.type="text/javascript",o.src=e;var a=function(){t(o),o.onreadystatechange=null,o.onload=null,o.onerror=null};return o.onload=a,o.onreadystatechange=function(){o.readyState in{loaded:1,complete:1}&&a()},o.onerror=n,o};this.script=function(n){return new t(function(t,r){e(function(){var e=u(n,t,r);o.firstChild?o.insertBefore(e,o.firstChild):o.appendChild(e)})})}}return n}),define("quant/event",[],function(){function e(){this.add=function(e,t,n){e.addEventListener?e.addEventListener(t,n):"function"==typeof jQuery?jQuery(e).on(t,n):e.attachEvent&&e.attachEvent("on"+t,n)},this.remove=function(e,t,n){e.removeEventListener?e.removeEventListener(t,n):"function"==typeof jQuery?jQuery(e).off(t,n):e.detachEvent&&e.detachEvent("on"+t,n)},this.trigger=function(e,t,n){var r=e.ownerDocument;if(e.dispatchEvent&&r.createEvent){var o=r.createEvent("Event");if(o.initEvent(t,!0,!0),void 0!==n)for(var a in n)a in o||(o[a]=n[a]);e.dispatchEvent(o)}else"function"==typeof jQuery&&jQuery(e).trigger(t,n)}}return new e}),define("quant/consent/truste",["quant/promise","quant/event","quant/dateutils"],function(e,t,n){return function(r,o,a,i,u,c,s,f){var l,p={},d=function(e){var t=e.source[0];return"p"+e.consent[0]+("a"==t?"e":"i")};l="object"==typeof i&&"function"==typeof i.callApi?function(t,n,r,o){var a=i.callApi(n,o,f,u,r);return t.cm=d(a),e.resolve(!0)}:function(i,c,s,l){return r.depth>0&&(t.add(o,"message",function(e){var t=e.data;if("string"==typeof t&&t.indexOf("PrivacyManagerAPI")>0)try{t=JSON.parse(t)}catch(e){return}else if(void 0!==t.PrivacyManagerAPI){var n=t.PrivacyManagerAPI;i.cm=d(n)}}),a.postMessage(JSON.stringify({PrivacyManagerAPI:{timestamp:n.nowTimestamp(),action:c,self:l,domain:f,authority:u,type:s}}),"*")),e.resolve(!0)},this.consent=function(e){return l(e,"getConsent",c,s)},this.parameters=p}}),define("quant/consent/uspapi",["quant/promise","quant/event","quant/dateutils"],function(e,t,n){return function(r,o,a,i){var u;if("function"==typeof o.__uspapi)u=function(t,n){return new e(function(e,t){o.__uspapi("getUSPData",n,function(n){n&&"string"==typeof n.uspString?e(n):t(n)})}).catch(function(e){return a.error("[USPAPI] unsuccessful: ",e),!0})};else{var c=(r.locate(i),{});t.add(o,"message",function(e){var t=e.data;if("string"==typeof t&&"{"==t[0])try{t=JSON.parse(t)}catch(e){return}if(Object.prototype.hasOwnProperty.call(t,"__uspapiReturn")){var n=t.__uspapiReturn,r=n.callId,o=c[r];if(void 0===o)return;n.success?o[0](n.returnValue):o[1](n.returnValue)}}),u=function(t,o){var a=r.locate(i);if(void 0===a)return e.resolve(void 0);var u=n.nowTimestamp();return new e(function(e,n){c[u]=[e,n],a.postMessage({__uspapiCall:{command:t,version:o,callId:u}})})}}this.consent=function(e){return u("getUSPData",1).then(function(t){return t&&"string"==typeof t.uspString&&(e.us_privacy=t.uspString),!0})}}}),define("quant/consent/manager",["quant/promise"],function(e){return function(t){var n,r={},o=function(o){return void 0===n&&(n=e.all(t.map(function(e){return e.consent(r)})).then(function(e){return e.reduce(function(e,t){return e&&t},!0)})),n.then(function(e){if(e)return o()})};this.consent=o,this.wrap=function(e){return function(){var t=this,n=arguments;return o(function(){return e.apply(t,n)})}},this.parameters=r}}),define("quant/consent/tcf2.0",["quant/promise","quant/event","quant/dateutils"],function(e,t,n){function r(e,t){var n=t.gdprApplies,r=t.purpose,o=t.vendor,a=o&&o.consents&&o.consents[c],i=o&&o.legitimateInterests&&o.legitimateInterests[c],u=t.publisher?t.publisher.restrictions:{};return!n||e.map(function(e){var t=!!r.consents&&r.consents[e],n=!!r.legitimateInterests&&r.legitimateInterests[e],o=u&&u[e]?u[e][c]:null;return 0!==o&&(!(!a||!t||2===o||-1==f.indexOf(e)&&1!==o)||!(1===o||!i||!n||-1!=l.indexOf(e)||-1!=f.indexOf(e)&&2!==o))}).reduce(function(e,t){return e&&t},!0)}function o(o,c,f,l){var g,m;if("function"==typeof c.__tcfapi)m=function(t,n){return new e(function(e,r){c.__tcfapi(t,p,function(n,o){if(o){var a=n.eventStatus;t===d&&n.gdprApplies&&"useractioncomplete"!==a&&"tcloaded"!==a||e(n)}else r(n)},n)})};else{var y={},q={};t.add(c,"message",function(e){var t=e.data;if(void 0===t)return void f.error(a+"Recieved undefined message");if("string"==typeof t&&"{"==t[0])try{t=JSON.parse(t)}catch(e){return}if(Object.prototype.hasOwnProperty.call(t,h)){var n=t[h],r=n.callId,o=y[r];if(void 0===o)return;var c=n.returnValue;n.success?q[r]===d&&c.gdprApplies&&"useractioncomplete"!==c.eventStatus&&"tcloaded"!==c.eventStatus||o[i](c):o[u](c)}}),m=function(t,r){var a=o.locate(l);if(void 0===a)return e.resolve({gdprApplies:!1});var i=n.nowTimestamp();return new e(function(e,n){y[i]=[e,n],q[i]=t;var o={};o[v]={command:t,parameter:r,version:p,callId:i},a.postMessage(JSON.stringify(o),"*")})}}this.consent=function(e){return void 0===g&&(g=m(d).then(function(t){return t.gdprApplies&&"false"!=t.gdprApplies?(e.gdpr=1,e.gdpr_consent=t.tcString):e.gdpr=e.gdpr||0,r(s,t)}).catch(function(t){return f.error("[TCF2.0] unsuccessful: ",t),e.gdpr=e.gdpr||0,!0})),g}}var a="[TCF2]: ",i=0,u=1,c=11,s=["1","3","7","8","9","10"],f=["1","3"],l=["1","3"],p=2,d="addEventListener",h="__tcfapiReturn",v="__tcfapiCall";return o.resolveConsent=r,o}),define("quant/qtrack",[],function(){function e(e,t){for(var n in t)Object.prototype.hasOwnProperty.call(t,n)&&(e[n]=t[n])}function t(t,n,a){for(var i=0;i<r.length;i++){var u={qacct:r[i],labels:a?t:"_fp.event."+t,event:"refresh"};if(e(u,o),void 0!==n&&null!==n)for(var c in n)Object.prototype.hasOwnProperty.call(n,c)&&("product_id"===c&&n[c].constructor===Array&&(n[c]=n[c].join(",")),u[s[c]||c]=n[c]);window._qevents.push(u)}}function n(n,s,f){if(n===i){if(-1!==r.indexOf(s))return;r.push(s);var l={qacct:s};e(o,f),e(l,o),window._qevents.push(l)}else n===u?-1!==a.indexOf(s)?t(s,f,!1):console.warn("Unsupported event by track, please use "+c+" for this event."):n===c&&t(s,f,!0)}var r,o,a=["PageView","ViewContent","Search","AddToWishlist","AddToCart","InitiateCheckout","AddPaymentInfo","Purchase","Lead","Register","StartTrial","Subscribe","SubmitApplication"],i="init",u="track",c="trackCustom",s={order_id:"orderid",value:"revenue"};return function(){if(window.qtrack||(window.qtrack=function(){window.qtrack.impl.apply(window.qtrack,arguments)}),!window.qtrack.impl&&(r=[],o={},window.qtrack.impl=n,window.qtrack&&window.qtrack.q))for(;window.qtrack.q.length>0;)n.apply(n,window.qtrack.q.shift())}}),define("quant/hashing",[],function(){function e(){function e(e){return unescape(encodeURIComponent(e))}function t(e,t){return t>>>e|t<<32-e}function n(e){return t(2,e)^t(13,e)^t(22,e)}function r(e){return t(6,e)^t(11,e)^t(25,e)}function o(e){return t(7,e)^t(18,e)^e>>>3}function a(e){return t(17,e)^t(19,e)^e>>>10}function i(e,t,n){return e&t^~e&n}function u(e,t,n){return e&t^e&n^t&n}this.FNV=function(e){var t,n,r,o;return t=2166136261,n=3386659096,r=c(t,e),o=c(n,e),Math.round(Math.abs(r*o)/65536).toString(16)};var c=function(e,t){var n;for(n=0;n<t.length;n++)e^=t.charCodeAt(n),e+=(e<<1)+(e<<4)+(e<<7)+(e<<8)+(e<<24);return e};this.SHA256=function(t){t=e(t);var c=[1116352408,1899447441,3049323471,3921009573,961987163,1508970993,2453635748,2870763221,3624381080,310598401,607225278,1426881987,1925078388,2162078206,2614888103,3248222580,3835390401,4022224774,264347078,604807628,770255983,1249150122,1555081692,1996064986,2554220882,2821834349,2952996808,3210313671,3336571891,3584528711,113926993,338241895,666307205,773529912,1294757372,1396182291,1695183700,1986661051,2177026350,2456956037,2730485921,2820302411,3259730800,3345764771,3516065817,3600352804,4094571909,275423344,430227734,506948616,659060556,883997877,958139571,1322822218,1537002063,1747873779,1955562222,2024104815,2227730452,2361852424,2428436474,2756734187,3204031479,3329325298],s=[1779033703,3144134277,1013904242,2773480762,1359893119,2600822924,528734635,1541459225];t+=String.fromCharCode(128);for(var f=t.length/4+2,l=Math.ceil(f/16),p=new Array(l),d=0;d<l;d++){p[d]=new Array(16);for(var h=0;h<16;h++)p[d][h]=t.charCodeAt(64*d+4*h+0)<<24|t.charCodeAt(64*d+4*h+1)<<16|t.charCodeAt(64*d+4*h+2)<<8|t.charCodeAt(64*d+4*h+3)<<0}var v=8*(t.length-1)/Math.pow(2,32),g=8*(t.length-1)>>>0;for(p[l-1][14]=Math.floor(v),p[l-1][15]=g,d=0;d<l;d++){for(var m=new Array(64),y=0;y<16;y++)m[y]=p[d][y];for(y=16;y<64;y++)m[y]=a(m[y-2])+m[y-7]+o(m[y-15])+m[y-16]>>>0;var q=s[0],w=s[1],_=s[2],b=s[3],x=s[4],O=s[5],E=s[6],S=s[7];for(y=0;y<64;y++){var j=S+r(x)+i(x,O,E)+c[y]+m[y],C=n(q)+u(q,w,_);S=E,E=O,O=x,x=b+j>>>0,b=_,_=w,w=q,q=j+C>>>0}s[0]=s[0]+q>>>0,s[1]=s[1]+w>>>0,s[2]=s[2]+_>>>0,s[3]=s[3]+b>>>0,s[4]=s[4]+x>>>0,s[5]=s[5]+O>>>0,s[6]=s[6]+E>>>0,s[7]=s[7]+S>>>0}for(S=0;S<s.length;S++)s[S]=("00000000"+s[S].toString(16)).slice(-8);return s.join("")}}return new e}),define("quant/normalize",["quant/hashing"],function(e){var t=function(e){var t=typeof e;return"string"==t&&e.length>0||"number"==t||"boolean"==t};return function(n,r,o,a,i){var u,c={},s=null,f=/^[^\s@]+@[^\s@]+\.[^\s@]+$/,l=/^[A-Fa-f0-9]{64}$/,p=2;for(u in r)Object.prototype.hasOwnProperty.call(r,u)&&t(r[u])&&("uid"!==u&&"uh"!==u?"qacct"!==u&&(c[u+n]=encodeURIComponent(r[u])):(r[u].match(l)?(p=1,s=r[u].toLowerCase()):(r[u].match(f)&&(p=0,r[u]=r[u].toLowerCase()),""!==r[u]&&(s=e.SHA256(r[u]))),delete r[u]));for(u in o)Object.prototype.hasOwnProperty.call(o,u)&&t(o[u])&&!c[u+n]&&(c[u+n]=encodeURIComponent(o[u]));return c["rf"+n]=""+i,"string"==typeof s&&(r.uh=s,c["uh"+n]=encodeURIComponent(s)),c["uht"+n]=""+p,c["a"+n]=a,c}}),define("quant/storage-experiment",["quant/dateutils"],function(e){function t(e){var t=document.cookie.match("(^|;)\\s*"+e+"\\s*=\\s*([^;]+)");return t&&t.pop()||""}function n(t,n,r){var o=new Date;o.setTime(e.nowTimestamp()+r),document.cookie=t+"="+n+";expires="+o.toUTCString()+";path=/"}function r(){try{var t=window.localStorage.getItem(i);if(t){var n=e.nowTimestamp()-parseInt(t);return Math.floor(n/1e3)}return window.localStorage.setItem(i,""+e.nowTimestamp()),0}catch(e){return-1}}function o(r){try{var o=t(u);if(o){var a=e.nowTimestamp()-parseInt(o);return Math.floor(a/1e3)}return n(u,e.nowTimestamp(),r),0}catch(e){return-1}}function a(e){var t=r(),n=o(e),a=[];return t>0&&a.push("sxl="+t),n>0&&a.push("sxc="+n),a.length>0?";"+a.join(";"):""}var i="qcSxl",u="qcSxc";return{getUrlParameters:a}}),define("quant/session",["quant/dateutils"],function(e){var t=function(){var t=e.nowTimestamp();return"xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx".replace(/[xy]/g,function(e){var n=(t+16*Math.random())%16|0;return t=Math.floor(t/16),("x"==e?n:3&n|8).toString(16)})};if(!window.sessionStorage)return t();var n=window.sessionStorage.getItem("qcSes");return n||(n=t(),window.sessionStorage.setItem("qcSes",n),n)}),define("src/quant.js",["quant/origin","quant/windows","quant/log","quant/loader","quant/consent/truste","quant/consent/uspapi","quant/consent/manager","quant/consent/tcf2.0","quant/qtrack","quant/normalize","quant/hashing","quant/storage-experiment","quant/session","quant/dateutils"],function(e,t,n,r,o,a,i,u,c,s,f,l,p,d){return void 0===window.__qc&&(window.__qc=function(c,h,v){if(c.__qc)return c.__qc;var g,m,y,q,w,_,b,x,O,E,S,j,C,I,P,T,A,k,D,R,M,U,z,L,N,V,B,J,Q,F,G,H,$,W,Z,K,X,Y,ee,te,ne,re,oe,ae,ie,ue,ce,se=e(h),fe=new t(c,c.top),le=new r(c,h),pe=new n(le,"quantcount.com"),de=new i([new o(fe,c,c.top,c.PrivacyManagerAPI,"truste.com","advertising","quantserve.com",se),new a(fe,c,pe,"__uspapiLocator"),new u(fe,c,pe,"__tcfapiLocator")]),he=["a","ce","cm","dst","enc","fpa","fpan","je","ns","ogl","rf","tzo","sr","sxl","sxc","ses"],ve=["4dcfa7079941","127fdf7967f31","588ab9292a3f","32f92b0727e5","22f9aa38dfd3","a4abfe8f3e04","18b66bc1325c","958e70ea2f28","bdbf0cb4bbb","65118a0d557","40a1d9db1864","18ae3d985046","3b26460f55d"],ge=!1,me=!1,ye=0,qe=[],we=[],_e=[],be=[],xe={},Oe=0,Ee=null,Se={},je={},Ce=null,Ie=[].slice;!function(){var e;e=h.createElement("script"),g="async"in e?1:e.readyState?2:3,e=null}();var Pe=function(e){try{return{init:x,hash:f.SHA256,push:O,rules:H,require:require,hasRules:W,defaults:Y,fire:T,__qc:function(){return!0}}[e].apply(null,Ie.call(arguments,1))}catch(e){return pe.error(e),!1}};return Pe.evts=0,Pe.v=2,Pe.SD=ve,Pe.qpixelsent=[],G=function(e){var t,n=e?e.length||0:0;for(t=0;t<n;t++)if(!e[t])return!1;return!0},X=function(e){(e=e||c._qacct)&&(L(be,e)||be.push(e))},L=function(e,t){var n,r=e.length;for(n=0;n<r;n++)if(e[n]===t)return!0;return!1},B=function(e){return{}.toString.call(e).match(/\s([a-zA-Z]+)/)[1].toLowerCase()},J=function(e){var t,n,r;if("array"===(n=B(e)))return e.slice(0);if("object"===n){t={};for(r in e)Object.prototype.hasOwnProperty.call(e,r)&&(t[r]=e[r]);return t}return"string"===n?""+e:e},O=function(e,t,n){C(e,t,n)},W=function(e){return L(_e,e)},Y=function(e,t){var n;e&&(n=Se[e],n&&(t=U(t,n)),t.qacct&&delete t.qacct,Se[e]=t)},ee=function(e){var t,n,r,o,a,i;if(y(e)){a=e;for(o in a){if("string"==typeof a[o]){t=e.event||"load",n=e.media||"webpage","rule"!==t&&"load"!==t||"webpage"!==n&&"ad"!==n?C(e):(i=e.qacct||c._qacct,e.qacct=i,r=je[i],r=r?U(r,e):e,je[i]=r),X(e.qacct);break}"object"==typeof a[o]&&null!=a[o]&&ee(a[o])}}},U=function(e,t){var n={};return n.qacct=e.qacct||t.qacct,"load"===e.event||"load"===t.event?n.event="load":e.event&&t.event?n.event=e.event||t.event:n.event=null,n.media=null,"webpage"===e.media||"webpage"===t.media?n.media="webpage":"ad"===e.media||"ad"===t.media?n.media="ad":n.media=e.media||t.media,z(n,e,t),z(n,t,e),n.event||delete n.event,n.media||delete n.media,n},z=function(e,t,n){var r,o,a,i,u,c;for(r in t)Object.prototype.hasOwnProperty.call(t,r)&&!Object.prototype.hasOwnProperty.call(e,r)&&(o=t[r],a=n[r],i="",u=!!o&&"string"==typeof o,c=!!a&&"string"==typeof a,u&&(i=o),u&&c&&(i+=","),c&&(i+=a),e[r]=i)},te=function(){var e,t,n=[];if(!(Oe>0)){V();for(e in je)Object.prototype.hasOwnProperty.call(je,e)&&je[e]&&(t=je[e],n.push(t),delete je[e]);1==n.length&&C(n[0]),n.length>1&&C(n)}},ne=function(){var e,t,n,r=[];for(n=be.slice(0),e=0;e<n.length;e++)t=n[e],W(t)||r.push(t);if(0===r.length)te();else for(e=0;e<r.length;e++)t=r[e],_e.push(t),R(t)},M=function(e,t,n,r){var o;e=c.location.protocol+"//"+e,Ee=h.scripts&&h.scripts[0]||null;var a=Ee&&Ee.parentNode||h.head||h;if(o=h.createElement("script"),1===g)o.src=e,o.async=!0,o.onload=t,n&&(o.onerror=function(e){o.onerror=null,n(e)}),a.insertBefore(o,Ee);else if(2===g){var i=!1;o.onload=o.onreadystatechange=function(){i||"loaded"!=o.readyState&&"complete"!=o.readyState||(i=!0,o.onreadystatechange=null,t())},o.src=e,a.insertBefore(o,Ee)}else r&&r()},R=function(e){Oe++,M("rules.quantcount.com/rules-"+e+".js",function(){xe[e]=2===g?2:0,re()},function(){xe[e]=1,re()},function(){xe[e]=4,re()})},re=function(){Oe-=Oe>0?1:0,te()},H=function(){var e,t,n,r=!0,o=!1;if(arguments.length){for(n=function(e){r?ee(e):C(e,!0),o=!0},e=0;e<arguments.length;e++)t=Ie.call(arguments[e],0),t.splice(1,0,n),$.apply(null,t);r=!1,ge&&te()}return o},$=function(e,t){var n,r,o,a,i,u,c,s=[],f=[],l=t||C;if((r=Ie.call(arguments,2))&&r.length){for(o=r[0]||G,a=r[1],i=r[2],n=i.length,u=0;u<n;u++)s.push(!1),f.push(null);c={p:e,f:s,r:o,c:i,a:a,v:f},W(e)||_e.push(e),we.push(c),Z(c,l)}else _e.push(e),xe[e]=6},Z=function(e,t){var n,r=e&&e.c?e.c.length:0;for(n=0;n<r;n++)!function(n){var r,o;try{r=e.c[n][0],o=e.c[n].slice(1),o.splice(0,0,function(r){e.f[n]=!0,e.v[n]=r,K(e,t)}),r.apply(null,o)}catch(r){e.f[n]=!0,e.v[n]=!1,K(e,t)}}(n)},K=function(e,t){var n,r,o,a,i,u,c,s=e.a,f=e.f,l=e.v,p=e.r||G;if(n=G(f),n&&(n=n&&p(l)),n)for(i=0;i<s.length;i++)try{r=s[i][0],o=s[i].length>1?s[i].slice(1):[],o=o.concat(e.v),a=r.apply(null,o),u={qacct:e.p,event:"rule"};for(c in a)Object.prototype.hasOwnProperty.call(a,c)&&"qacct"!==c&&(u[c]=a[c]);t(u)}catch(e){continue}},m=function(e){return e.replace(/\./g,"%2E").replace(/,/g,"%2C")},y=function(e){return void 0!==e&&null!=e},q=function(){return Math.round(2147483647*Math.random())},w=function(e){var t,n,r,o="",a=h.cookie;return a?(t=a.indexOf(e+"="),n=t+e.length+1,t>-1&&(r=a.indexOf(";",n),r<0&&(r=a.length),o=a.substring(n,r)),o):o},j=function(e){return"P0-"+q()+"-"+e.getTime()},S=function(e,t,n){return["__qca=",e,"; expires=",t.toGMTString(),"; path=/; domain=",n].join("")},E=function(){var e,t,n,r,o,a,i,u,c,s=["","",""];if(1===ye)return s[0]=";fpan=u;fpa=",s;for(e=f.FNV(se),r=0;r<ve.length;r++)if(ve[r]===e)return s[0]=";fpan=u;fpa=",s;return t=d.now(),n=w("__qca"),n.length>0||Ce?(0===n.length&&(n=Ce,s[1]=S(Ce,new Date(t.getTime()+338688e5),se)),s[0]=";fpan=0;fpa="+n):(Ce=j(t),s[1]=S(Ce,new Date(t.getTime()+338688e5),se),s[0]=";fpan=1;fpa="+Ce),o=w("_pubcid"),a=w("_pubcid_optout"),i=o.length>0?o:w("_sharedID"),u=w("_pbjs_id_optout"),c=u.length>0||"1"===a,!c&&i.length>0?s[2]=";pbc="+i:s[2]=";pbc=",s},_=function(){var e=E()[1];e&&(h.cookie=e)},b=function(e){h.cookie=e+"=; expires=Thu, 01 Jan 1970 00:00:01 GMT; path=/; domain="+se},F=function(e){var t,n;if(e&&"object"===B(e))for(n=0;n<he.length;n++)t=he[n],Object.prototype.hasOwnProperty.call(e,t)&&e[t]&&delete e[t]},k=function(e,t,n){var r,o,a;return t&&"string"==typeof t.qacct?r=t.qacct:"string"==typeof c._qacct&&(r=c._qacct),r&&0!==r.length?(t=Q(r,t),delete je[r],a=Se[r],o=xe[r],y(o)||(o=3),ce(t,a,n,r)?null:s(e,t,a,r,o)):null},A=function(e){var t,n=[],r=[],o=[];for(t in e)e[t]&&Object.prototype.hasOwnProperty.call(e,t)&&("uh"===t||"uht"===t?r.push(";"+t+"="+e[t]):n.push(t+"="+e[t]));return o.push(n.join(";")),o.push(r.join("")),o},D=function(){var e,t,n,r,o,a=h.getElementsByTagName("meta"),i="";for(e=0;e<a.length;e++){if(o=a[e],i.length>=1e3)return encodeURIComponent(i);y(o)&&y(o.attributes)&&y(o.attributes.property)&&y(o.attributes.property.value)&&y(o.content)&&(t=o.attributes.property.value,n=o.content,t.length>3&&"og:"===t.substring(0,3)&&(i.length>0&&(i+=","),r=n.length>80?80:n.length,i+=m(t.substring(3,t.length))+"."+m(n.substring(0,r))))}return encodeURIComponent(i)},C=function(e,t,n){var r,o,a,i,u,s,f,g,m,w,_,b,x=q(),O="",S="",j="",C="",I="1",T=[];if(ye=0,y(Pe.qpixelsent)||(Pe.qpixelsent=[]),y(e)){if("object"===(w=B(e)))a=k("",e,t);else if("array"===w)for(u=0;u<e.length;u++)_=k("."+(u+1),e[u],t),a=0===u?_:U(a,_)}else"string"==typeof _qacct&&(a=k("",null,t));if(a){r=v.cookieEnabled?"1":"0",y(c._qmeta)&&(O=";m="+encodeURIComponent(c._qmeta),c._qmeta=null),s=d.now(),f=d.isDst(),b=E(),g=b[0],m=b[2],c.location&&c.location.href&&(S=encodeURIComponent(c.location.href)),h&&h.referrer&&(C=encodeURIComponent(h.referrer)),c.self===c.top&&(I="0"),a.url?j=S:a.url=S,a.ref||(a.ref=C||""),o=D(),i=A(a);var R=ue(n,a.event),M="engagement"===a.event?"/engagement":"/pixel";T.push(M+R+"r="+x+";"+i[0]),T.push(i[1]),T.push(g+m),T.push(";ns="+I+";ce="+r+";qjs=1;qv=d18171e5-20220913105912"),T.push((a.ref?"":";ref=")+";d="+se+";dst="+f+";et="+s.getTime()+";tzo="+s.getTimezoneOffset()+(j?";ourl="+j:"")+O+";ogl="+o+l.getUrlParameters(338688e5)+";ses="+p),qe.push({pixel:T,target:n}),P()}},ue=function(e,t){return"engagement"===t?"?":void 0===e?";":"?"},I=function(e){var t=e.pixel;de.consent(function(){return!0}).then(function(e){return e||b("__qca"),e?"quantserve.com":"quantcount.com"}).then(function(n){var r,o=de.parameters,a=function(){return function(){return"quantserve.com"===n?[t[1],t[2]].join(""):";uh=u;uht=u"}}(),i=y(e.target)?e.target:"https://pixel."+n,u=[i,t[0],a(),t[3],";cm=",o.cm,1===o.gdpr?";gdpr=1;gdpr_consent="+o.gdpr_consent:";gdpr=0",o.us_privacy?";us_privacy="+o.us_privacy:"",t[4]].join("");return"function"!=typeof CustomEvent?(r=document.createEvent("CustomEvent"),r.initCustomEvent("q_pixel_fire",!1,!1,{url:u})):r=new CustomEvent("q_pixel_fire",{detail:{url:u}}),c.dispatchEvent(r),le.image(u).then(function(e){e&&"number"==typeof e.width&&3===e.width?b("__qca"):"quantserve.com"===n&&_()})})},P=function(){for(;qe.length;)I(qe.shift())},T=function(e){var t,n,r,o,a,i,u,s=q(),f="",l="",p="1",g=[];t=v.cookieEnabled?"1":"0",r=new Date,o=d.isDst(),u=E(),a=u[0],i=u[2],c.location&&c.location.href&&(f=encodeURIComponent(c.location.href)),h&&h.referrer&&(l=encodeURIComponent(h.referrer)),c.self===c.top&&(p="0"),n=D(),g.push("/pixel?r="+s),g.push(a+i),g.push(";ns="+p+";ce="+t+";qjs=1;qv=d18171e5-20220913105912"),g.push(";ref="+l,";d="+se+";dst="+o+";et="+r.getTime()+";tzo="+r.getTimezoneOffset()+(f?";url="+f:"")+";ogl="+n),qe.push({pixel:g,target:e}),P()},oe=function(){var e,t,n=arguments;for(N([].slice.call(n)),t=0;t<n.length;t++)e=n[t],C(e);be.length?ne():te()},N=function(e){var t,n=B(e);if("array"===n)for(t=0;t<e.length;t++)N(e[t]);else"object"===n&&X(e.qacct||c._qacct)},V=function(){var e;if(me||c._qevents.length||c.ezt.length||"undefined"==typeof _qacct||(C({qacct:c._qacct}),me=!0),!Pe.evts){for(e in c._qevents)c._qevents[e]!==c._qevents.push&&Object.prototype.hasOwnProperty.call(c._qevents,e)&&C(c._qevents[e]);for(e in c.ezt)c.ezt[e]!==c.ezt.push&&Object.prototype.hasOwnProperty.call(c.ezt,e)&&C(c.ezt[e]);c._qevents={push:oe},c.ezt.push=function(){var e,t=arguments;if(y(c.queueManager))for(e=0;e<t.length;e++)c.queueManager.push(t[e]);else oe.apply(this,arguments)},Pe.evts=1}},ie=function(e){var t;e&&(t=J(e),N(e),c._qevents.push(t),e=null)},ae=function(e){e.push=function(){return N([].slice.call(arguments)),ne(),[].push.apply(e,arguments)}},ce=function(e,t,n,r){t=t||{};var o=(e?e.media:t.media)||"webpage",a=(e?e.event:t.event)||"load";if("ad"===o&&(ye=1),"webpage"===o&&"load"===a){for(var i=0;i<Pe.qpixelsent.length;i++)if(Pe.qpixelsent[i]===r&&!n)return!0;Pe.qpixelsent.push(r)}return!1},Q=function(e,t){var n=je[e];return t?n&&(t=U(t,n)):t=n,F(t),t},x=function(){y(c._qevents)||(c._qevents=[]),y(c.ezt)||(c.ezt=[]),ie(c._qoptions),ie(c.qcdata),ie(c.smarttagdata),Pe.evts||(ae(c._qevents),ae(c.ezt)),N(c.ezt),N(c._qevents),N({qacct:c._qacct}),c._qoptions=null,be.length?ne():te(),ge=!0},c.quantserve=c.quantserve||x,Pe.quantserve=x,Pe}(window,window.document,window.navigator)),window.quantserve(),c(),window.__qc})}(window);