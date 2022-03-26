/// <reference path="../../../typings/globals/jquery/index.d.ts" />

$(document).ready(async function(){
	$("#reset").click(() => $("#ssid, #pass").val(""));

	$("#ssid, #pass").keypress(e => {
		if(e.key == "Enter")
			$("#submit").trigger("click");
	});

	$("#submit").click(async () => {
		const ssid = $("#ssid").val();
		const pass = $("#pass").val();
		
		if(ssid == ""){
			alert("Inserisci un SSID valido!");
			return;
		}

		if(pass == ""){
			alert("Inserisci una password valida!");
			$("#pass").focus();
			return;
		}

		$.post("/wifi", {
			ssid:	ssid,
			pass:	pass
		});

		alert("Connessione in corso; il dispositivo verra' riavviato.");
	});

	$("#refresh").click(async () => {
		$(".loading").show();
		
		let nets;
		try{
			nets = await $.get("/networks");
			// nets = [{ ssid: "test", rssi: 3 }, { ssid: "test1", rssi: 4 }];
		}
		catch(e){
			alert("Errore " + e.status + ": " + e.statusText + "; ricaricare la pagina.");
			return;
		}

		if(nets == null){
			$(".loading").hide();
			return;
		}

		let str = "";
		nets.forEach(e => {
			str += 	"<div class='network'>";
			str += 		"<div onclick='alert(\"" + e.ssid + "\")'>";
			str += 			"<h3>" + (e.ssid.length > 10 ? e.ssid.substring(0, 9) + "..." : e.ssid) + "&nbsp;</h3>";
			str += 			"<h5>(" + e.rssi + "%)</h5>";
			str += 		"</div>";
			str += 		"<div class='button' onclick=\"select('" + e.ssid + "');\"> Connetti </div>";
			str += 	"</div>";
		});

		$(".network").remove();
		$(".loading")
			.hide()
			.before(str);
	});

	$("#refresh").trigger("click");
});

function select(ssid){
	$("#ssid").val(ssid);
	$("#pass").focus();
}
