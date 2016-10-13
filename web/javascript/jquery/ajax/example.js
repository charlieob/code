$(document).ready(function(){
  $("button").click(changeContent);
});
function changeContent() {
  $("#myDiv").load('ajax_info.txt');
}
