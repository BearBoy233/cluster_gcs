/**/_jsload2&&_jsload2('scommon', 'var sh=new P(23,25),th=new P(9,25),uh=new P(9,0);W.OT=function(a,b,c,e){var f=vh(b);if(f&&!(0>c||9<c)){b=s;f=new V(f);if(I()){var g=function(){return new qc(H.sa+"markers_hd.png",new P(22,31),{anchor:new P(11,31),imageOffset:new P(0,-32*c),imageSize:new P(85,477),infoWindowAnchor:new P(11,0)})},b=g(),g=g();g.jt(new P(-28,-32*c));f.nD=b;f.$K=g}else b=new qc(H.sa+"markers.png",sh,{anchor:th,imageOffset:new P(0,-25*c),infoWindowAnchor:uh});f.Tb(b);a.Pa(f);e&&f.Dc(e);return f}}; W.Lv=function(a,b,c){if(b=vh(b)){var b=new V(b),e=s;I()?(e=new qc(H.sa+"markers_hd.png",new P(21,31),{anchor:th,imageOffset:new P(-29,-320),imageSize:new P(85,477),infoWindowAnchor:uh}),b.nD=e):e=new qc(H.sa+"markers.png",sh,{anchor:th,imageOffset:new P(0,-250),infoWindowAnchor:uh});b.Tb(e);a.Pa(b);c&&b.Dc(c);return b}}; W.NT=function(a,b,c){if(b=vh(b)){var b=new V(b),e=s;if(I()){var f=function(){return new qc(H.sa+"markers_hd.png",new P(21,31),{anchor:th,imageOffset:new P(0,-383),imageSize:new P(85,477),infoWindowAnchor:uh})},e=f(),f=f();f.jt(new P(-29,-352));b.nD=e;b.$K=f}else e=new qc(H.sa+"markers.png",sh,{anchor:th,imageOffset:new P(0,-300),infoWindowAnchor:uh});b.Tb(e);a.Pa(b);c&&b.Dc(c);return b}}; function vh(a){var b=[],b=s;if(a instanceof J)b=a;else{if("string"==typeof a){b=z.trim(a).split(",");if(2>b.length)return;b[0]=parseFloat(z.trim(b[0]));b[1]=parseFloat(z.trim(b[1]))}else if(b=a.slice(0),2>b.length)return;b=new J(b[0],b[1])}return b} W.ZU=function(a){var b=a.title,c=a.tr,e=a.k_,f=a.jV,a=a.SY||0;if(!b)return s;var g=b;1==a?g+="-\\u516c\\u4ea4\\u8f66\\u7ad9":3==a&&(g+="-\\u5730\\u94c1\\u7ad9");var i=13;f&&(i=10);g.replace(/[\\u0100-\\uffff]/g,"##").length>2*i+1&&(g=g.substring(0,i)+"&#8230");b="<p style=\'width:210px;font:bold 14px/16px "+H.fontFamily+";margin:0;color:#cc5522;white-space:nowrap;overflow:hidden\' title=\'"+b+"\'>"+g;f&&(b+="<a target=\'_blank\' href=\'"+f+"\' style=\'margin-left:5px;font-size:12px;color:#3d6dcc;font-weight:normal;text-decoration:none;\'>\\u8be6\\u60c5\\u00bb</a>"); b+="</p>";f=[];f.push("<div style=\'font:12px "+H.fontFamily+";margin-top:10px\'>");if(c){g="\\u5730\\u5740\\uff1a";if(1==a||3==a)g="\\u8f66\\u6b21\\uff1a";f.push(\'<table cellspacing="0" style="overflow:hidden;table-layout:fixed;width:100%;font:12px \'+H.fontFamily+\'" >\');f.push(\'<tr><td style="vertical-align:top;width:38px;white-space:nowrap;word-break:keep-all">\'+g+\'&nbsp;</td><td style="line-height:16px">\');f.push(c+"&nbsp;</td></tr>");e&&f.push(\'<tr><td style="vertical-align:top;">\\u7535\\u8bdd\\uff1a</td><td>\'+ e+"</td></tr>");f.push("</table>")}else e&&(f.push(\'<table cellspacing="0" style="table-layout:fixed;width:100%;font:12px \'+H.fontFamily+\'">\'),f.push(\'<tr><td style="vertical-align:top;width:36px;white-space:nowrap;word-break:keep-all">\\u7535\\u8bdd\\uff1a</td><td>\'+e+"</td></tr></table>"));f.push("</div>");return new tc(f.join(""),{title:b,height:0,width:230,margin:[10,10,20,10]})}; W.Xn=function(a,b,c,e){if(b=vh(b))return e=new V(b,{icon:I()?new qc(H.sa+"dest_mkr.png",new P(29,34),{anchor:new P(14,32),imageSize:new P(94,41),imageOffset:new P(-29*e,0),infoWindowAnchor:new P(14,0)}):new qc(H.sa+"dest_markers.png",new P(42,34),{anchor:new P(14,32),imageOffset:new P(0,-34*e),infoWindowAnchor:new P(14,0)}),baseZIndex:35E5,draggingCursor:"pointer"}),a.Pa(e),c&&e.Dc(c),e};W.Yn=function(a,b){return W.$n(a,b,Ld)}; W.MO=[{stroke:6,color:"#0030ff",opacity:0.45,style:"solid"},{stroke:5,color:"#00bd00",opacity:0.95,style:"solid"},{stroke:5,color:"#ffac00",opacity:0.95,style:"solid"},{stroke:5,color:"#f41c0d",opacity:0.95,style:"solid"}]; W.LO=[{stroke:6,color:"#5298fe",opacity:1,style:"solid"},{stroke:6,color:"#5298fe",opacity:1,style:"solid"},{stroke:6,color:"#4ddc26",opacity:1,style:"solid"},{stroke:6,color:"#00bd01",opacity:1,style:"solid"},{stroke:6,color:"#ff0103",opacity:0.65,style:"dashed"},{stroke:6,color:"#ff0103",opacity:0.65,style:"solid"},{stroke:6,color:"#5298ff",opacity:1,style:"solid"},{stroke:6,color:"#5298ff",opacity:1,style:"solid"},{stroke:6,color:"#ff0103",opacity:0.65,style:"dashed"}]; W.MT=function(a,b,c){var e=W.MO;"undefined"==typeof c&&(c=0);if(e[c])return e=e[c],b=new Gc(b,{strokeWeight:e.stroke,strokeColor:e.color,strokeOpacity:e.opacity,strokeStyle:e.style,clickable:t}),a.Pa(b),b.j1=c,b}; W.$n=function(a,b,c){var e=W.LO;"undefined"==typeof c&&(c=0);if(e[c]){var f=e[c],e=new Gc(b,{strokeWeight:f.stroke,strokeColor:f.color,strokeOpacity:f.opacity,strokeStyle:f.style,clickable:t});a.Pa(e);e.c1=c;c==Ld&&(b=new Gc(b,{strokeWeight:f.stroke-2,strokeColor:"#fff",strokeOpacity:0.3,clickable:t}),e.oA=b,a.Pa(b));return e}}; W.hw=function(a){var b=a.content,c=a.lw,e=a.total,f=a.tx,g=a.U3,a=a.pX,i=["<div style=\'font:12px "+H.fontFamily+"\'>"];b&&i.push("<div style=\'margin:10px 1em 24px 0\'>"+b+"</div>");a||i.push(W.cW(e,c));i.push("</div>");b=new tc(i.join(""),{title:"",height:0,width:220,margin:[10,10,20,10]});if(a)return b;b.addEventListener("open",function(){var a=I()?"touchend":"click";z.Ec("trans_prev")&&z.V(z.Ec("trans_prev"),a,function(){f&&"function"==typeof f&&f(c-1,g)});z.Ec("trans_next")&&z.V(z.Ec("trans_next"), a,function(){f&&"function"==typeof f&&f(c+1,g)})});return b}; W.cW=function(a,b){var c=[];c.push(\'<table id="trans_nav" width="99%" cols="2" style="background:#eaf2ff;font:12px \'+H.fontFamily+\'"><tbody><tr>\');c.push(\'<td nowrap="nowrap" width="50%" align="left">\');0==b?c.push(\'<span unselectable="on" style="color:#9c9d99;-moz-user-select:none" >\\u4e0a\\u4e00\\u6b65</span>\'):c.push(\'<a id="trans_prev" style="color:#7777cc;-moz-user-select:none" href="javascript:void(0)">\\u4e0a\\u4e00\\u6b65</a>\');c.push("</td>");c.push(\'<td nowrap="nowrap" width="50%" align="right">\'); b==a+1?c.push(\'<span unselectable="on" style="color:#9c9d99;-moz-user-select:none" >\\u4e0b\\u4e00\\u6b65</span>\'):c.push(\'<a style="color:#7777cc" id="trans_next" href="javascript:void(0)">\\u4e0b\\u4e00\\u6b65</a>\');c.push("</td></tr></tbody></table>");return c.join("")}; W.NI=function(a,b,c,e){if(b=vh(b)){c=c||0;if(I())c=new qc(H.sa+"dest_mkr.png",new P(20,20),{imageSize:new P(94,41),imageOffset:new P(-59,-21*c),infoWindowAnchor:new P(10,0)});else{var f=-55;switch(c){case 1:f=-76;break;case 3:f=-118;break;case 4:f=-139;break;default:f=-55}c=new qc(H.sa+"trans_icons.png",new P(21,21),{imageOffset:new P(0,f),infoWindowAnchor:new P(10,0)})}c=new V(b,{icon:c});a.Pa(c);e&&c.Dc(e);return c}};W.js=function(a){return a&&a.Qa&&a.Qa.jg?a.Qa.jg.childNodes[0]:s}; W.RT=function(a,b,c){b=new V(b,{icon:new qc(we.rT,new P(11,11))});c&&b.Dc(c);a.Pa(b);return b};W.JI=function(a,b){var c=new V(b,{icon:new qc(H.sa+"drag.png",new P(11,11)),draggingCursor:"pointer"});a.Pa(c);c.pp(10);return c};W.OI=function(a,b,c){b=new V(b,{icon:new qc(H.sa+"way-points.png",new P(36,40),{imageOffset:new P(-11+-36*c,-35)}),draggingCursor:"pointer",offset:new P(5,-20)});a.Pa(b);b.pp(10);return b}; W.wJ=function(){var a=new uc("\\u62d6\\u52a8\\u4ee5\\u66f4\\u6539\\u8def\\u7ebf",{offset:new P(30,-10)});a.Ld({border:"solid 1px gray",padding:"2px",whiteSpace:"nowrap",background:"#fff"});return a};W.nW=function(a,b){if(a&&b)return Math.round(Math.sqrt(Math.pow(a.x-b.x,2)+Math.pow(a.y-b.y,2)))}; W.PT=function(a,b,c,e){var f={type:"RouteAddrMarker"},a=vh(a),f=f||{};f.type=f.type||"normal";if(a&&!("undefined"==typeof b||0>b||9<b&&"child"!==f.type||39<b&&"child"===f.type)){var g;g=H.sa+"markers_new.png";var i=0;"RouteAddrMarker"==f.type&&(i=-H.lF);i=new qc(g,H.nF,{offset:H.kO,imageOffset:new P(-H.mF*b,i),infoWindowOffset:H.jF});g=b+1;b=new V(a,{icon:i,zIndexFixed:p,baseZIndex:25E5-100*b});b.L1={type:f.type};e.Pa(b);b.f1=g;c&&b.Dc(c);return b}};X.ac=1;X.Ap=2;X.Tm=3;X.Wt=4;X.Xt=5;X.Vt=6;X.Ut=7;X.sF=8;X.rF=9;X.Eu=function(a,b){return a?D.Uo+"?s="+encodeURIComponent("inf&uid="+a+"&c="+b)+"&i=0&sr=1":""};X.bR=function(a){return!a?-1:"string"==typeof a?2:a instanceof J?1:a instanceof Oa?0:"number"==typeof a?3:-1}; z.extend(X.prototype,{Eg:function(a,b){var c=X.bR(a.src);if(-1!=c){var e=this;if(3==c)b&&b(a.src);else if(2==c)a.HB==c&&a.IB==a.src&&a.Pl?b&&b(a.Pl):(a.HB=c,a.IB=a.src,Md.nb(function(a){a&&(a.result&&2==a.result.type)&&(e.yd.Pl=a.content.code,b&&b(e.yd.Pl))},{qt:"cur",wd:a.src}));else{var f,g=18;0==c?a.src.fx()?(f=a.src.Bb(),g=a.src.ka()):a.src.addEventListener("load",function(){e.Eg(a,b)}):f=a.src;c==a.HB&&a.IB.oc(f)&&a.Pl?b&&b(a.Pl):(0==c&&(f=S.Gb(f)),1==c&&(f=S.Gb(f)),a.HB=c,a.IB=new J(f.lng,f.lat), Md.nb(function(a){if(a&&a.result&&a.result.type==4){e.yd.Pl=a.content?a.content.uid:1;b&&b(e.yd.Pl)}},{qt:"cen",b:f.lng+","+f.lat+";"+f.lng+","+f.lat,l:g}))}}},Xa:function(a){"number"==typeof a?this.Ke=a:delete this.Ke},Ja:function(a,b,c){var e;e="undefined"===typeof b?4:b.Nq&&b.Nq.length?1:"[object Object]"===Object.prototype.toString.call(b.ov)&&"[object Object]"===Object.prototype.toString.call(b.zu)?3:2;switch(a){case X.ac:(a=this.m.Vs)&&a(b,e);break;case X.Ap:(a=this.m.lM)&&a(b,c);break;case X.Wt:(a= this.m.XD)&&a(b,c);break;case X.Tm:(a=this.m.kM)&&a(b,c);break;case X.Xt:(a=this.m.nM)&&a(b,c);break;case X.Vt:(a=this.m.jM)&&a(b,c);break;case X.Ut:(a=this.m.iM)&&a(b,c);break;case X.sF:(a=this.m.gM)&&a(b,c);break;case X.rF:(a=this.m.fM)&&a(b,c)}}});var wh=0,xh=1,yh=2;db.unique=function(a){for(var b=[],c={},e=0,f=a.length;e<f;e++)c[a[e]]||(c[a[e]]=p,b.push(a[e]));return b};db.Aw=function(a,b){if(b)var c=Math.min(b.width/1100,b.height/660),a=Math.round(a+Math.log(c)/Math.log(2));1>a&&(a=1);18<a&&(a=18);return a}; db.ze=function(a,b){if("string"==typeof a&&a){var c=a.split("|"),e,f,g;if(1==c.length)e=zh(a);else if(e=zh(c[2]),f=zh(c[0]),g=zh(c[1]),!b)return e;c={type:e.gW};if(b)switch(c.type){case yh:f=new J(e.Hd[0][0],e.Hd[0][1]);f=S.cc(f);c.point=f;c.na=[f];break;case xh:c.na=[];e=e.Hd[0];for(var i=0,k=e.length-1;i<k;i+=2){var m=new J(e[i],e[i+1]),m=S.cc(m);c.na.push(m)}f=new J(f.Hd[0][0],f.Hd[0][1]);g=new J(g.Hd[0][0],g.Hd[0][1]);f=S.cc(f);g=S.cc(g);c.fb=new hb(f,g)}return c}}; db.uM=function(a){var b;b?0.25>b?b=0:0.25<b&&1>b?b=1:32<b&&(b=32):b=0;var c=a.split("|");if(1==c.length){var e=zh(c[0]);return{type:e.type,bound:"",na:e.Hd.join(",")}}if(1<c.length){for(var f=a.split(";.="),a=[],g=[],i=0,k=f.length,c=0;c<k;c++){i=f[c];1<k&&(0==c&&(i+=";"),0<c&&c<k-1&&(i=".="+i+";"),c==k-1&&(i=".="+i));var i=i.split("|"),e=zh(i[0]),m=zh(i[1]);a.push(e.Hd.join(","));a.push(m.Hd.join(","));e=zh(i[2]);i=e.type;e=e.Hd.join(",");e=db.FY(e);0<b&&(e=db.GY(e,b));g.push(e)}1>=k&&(g=g.join(";")); if(2==k){b=(g[0]+";"+g[1]).split(";");f=[];for(c=0;c<b.length;c++)g=new J(b[c].split(",")[0],b[c].split(",")[1]),g=S.cc(g),f.push(g);g=f}return{type:i,bound:a.join(";"),na:g}}};db.zK=function(a){var b=[],b=s;if("Point"==a.toString())b=a;else{if("string"==typeof a){b=ja.trim(a).split(",");if(2>b.length)return;b[0]=parseFloat(ja.trim(b[0]));b[1]=parseFloat(ja.trim(b[1]))}else if(b=a.slice(0),2>b.length)return;b=new D.Vf(b[0],b[1])}return b};db.HY=function(a){a=a.split(",");a=new J(a[0],a[1]);return S.cc(a)}; db.pg={country:4,province:11,city:12,r1:13};var Ah=["=",".","-","*"],Bh=8388608; function zh(a){var b;b=a.charAt(0);var c=-1;b==Ah[1]?c=yh:b==Ah[2]?c=xh:b==Ah[3]&&(c=wh);b=c;for(var c=a.substr(1),e=0,f=c.length,g=[],a=[],i=[];e<f;)if(c.charAt(e)==Ah[0]){if(13>f-e)return 0;a:{for(var i=c.substr(e,13),k=g,m=0,n=0,o=0,q=0;6>q;q++){o=Ch(i.substr(1+q,1));if(0>o){i=-1-q;break a}m+=o<<6*q;o=Ch(i.substr(7+q,1));if(0>o){i=-7-q;break a}n+=o<<6*q}k.push(m);k.push(n);i=0}if(0>i)return 0;e+=13}else if(";"==c.charAt(e))a.push(g.slice(0)),g.length=0,++e;else{if(8>f-e)return 0;i=Dh(c.substr(e, 8),g);if(0>i)return 0;e+=8}c=0;for(e=a.length;c<e;c++){f=0;for(g=a[c].length;f<g;f++)a[c][f]/=100}return{gW:b,Hd:a}}function Dh(a,b){var c=b.length;if(2>c)return-1;for(var e=0,f=0,g=0,i=0;4>i;i++){g=Ch(a.substr(i,1));if(0>g)return-1-i;e+=g<<6*i;g=Ch(a.substr(4+i,1));if(0>g)return-5-i;f+=g<<6*i}e>Bh&&(e=Bh-e);f>Bh&&(f=Bh-f);b.push(b[c-2]+e);b.push(b[c-1]+f);return 0}function Ch(a){var b=a.charCodeAt(0);return"A"<=a&&"Z">=a?b-65:"a"<=a&&"z">=a?26+b-97:"0"<=a&&"9">=a?52+b-48:"+"==a?62:"/"==a?63:-1} db.Bx=function(a){var b=[];if("string"===typeof a)for(var a=a.split(";"),c=0;c<a.length;c++){var e=a[c].split(",");b.push(new J(e[0],e[1]))}return b}; ');