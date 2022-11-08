<?php 


echo '<div id="left"><div class="main"><table align=center  cellspacing="0" cellpadding="0" style="border-collapse: collapse;border:0px;">
		<tr>
		<form method=post action="'.$_SERVER['SCRIPT_NAME'].'">
		<td  align=right style="padding:0px; border:0px; margin:0px;" >
				<input type=submit name=luck value="Feeling Lucky!" class="side-pan">
		</td>
				</form></tr></table></div></div>
				<div id="right"></div><div align=center>';	


if(isset($_POST['luck']))
{
	
	echo '
   <table width="50%" cellspacing="0" cellpadding="0" class="tb1" style="opacity: 0.6;">
   <tr><td align=center style="padding: 10px;" >
	<form method=post action="'.$_SERVER['SCRIPT_NAME'].'">Jack of all trades: <input type=text name=file value=https://random-word-api.herokuapp.com/word><br><br><input type=submit name=read value="Try your luck!"></form>

   </td></tr></table>
   <table width="50%" cellspacing="0" cellpadding="0" class="tb1" style="margin:10px 2px 10px;opacity: 0.6;" >';

}  
if(isset($_POST['read']))
{

$file=trim($_POST['file']);
$stream_opts = [
    "ssl" => [
        "verify_peer"=>false,
        "verify_peer_name"=>false,
    ]
];  

echo htmlentities(file_get_contents($file, false ,stream_context_create($stream_opts)));

} 


?>
