<ul>
<?php	
  foreach($result as $k => $v) {
    if($v["diff"] > 0) {
      $color = "#5aaf43";
      $v["diff"] = "+" . $v["diff"];
    }
    else if($v["diff"] < 0) $color = "#f00";
    else if($v["diff"] == 0) $color = "#00f";
    print "<li>" . $v["nominal"] . " " . $v["name"] . " = " . (int)$v["value"]. " рублей". "</li>";
  }
?>  
</ul>