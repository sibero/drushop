<?php
GLOBAL $base_url;

if (function_exists('curl_init')) {
// http://www.web-junior.net/otpravka-post-zaprosov-s-pomoshhyu-php-chast-2/
 //инициализируем сеанс
$curl = curl_init();
//уcтанавливаем урл, к которому обратимся
curl_setopt($curl, CURLOPT_URL, 'http://drushop.ru/server.php');
//включаем вывод заголовков
curl_setopt($curl, CURLOPT_HEADER, 1);
//передаем данные по методу post
curl_setopt($curl, CURLOPT_POST, 1); 
//теперь curl вернет нам ответ, а не выведет
curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1); 
//переменные, которые будут переданные по методу post

curl_setopt($curl, CURLOPT_POSTFIELDS, 'server='.$base_url.'&ip='.$_SERVER['SERVER_ADDR'].'&name='.variable_get('site_name', 'не указано'));
//я не скрипт, я браузер опера
curl_setopt($curl, CURLOPT_USERAGENT, 'Opera 10.00');
$res = curl_exec($curl);
 
curl_close($curl);
}

else {
if (function_exists('fsockopen')) {
// http://www.web-junior.net/otpravka-post-zaprosov-s-pomoshhyu-php-chast-1/
//открываем сокет к http://www.example.loc на 80-й порт с таймаутом в 30 секунд
$socket = fsockopen('drushop.ru', 80, $errno, $errstr, 30);
 
//если fsockopen вернула false, то завершаем работу скрипта и выводим текст и номер ошибки
if(!$socket)die("$errstr($errno)");
 
//собираем данные
$data = "server=".urlencode($base_url)."&ip=".urlencode($_SERVER['SERVER_ADDR'])."&name=".urlencode(variable_get('site_name', 'не указано'));
 
//пишем в сокет метод, URI и протокол 
fwrite($socket, "POST /server.php HTTP/1.1\r\n");
//а также имя хоста
fwrite($socket, "Host: drushop.ru\r\n");
 
//теперь отправляем заголовки
//Content-type должен быть applicaion/x-www-form-urlencoded
fwrite($socket,"Content-type: application/x-www-form-urlencoded\r\n");
//размер передаваемых данных передаем в заголовке Content-length
fwrite($socket,"Content-length:".strlen($data)."\r\n");
//типы принимаемых данных. */* означает, что принимаем все типы данных
fwrite($socket,"Accept:*/*\r\n");
//представимся оперой
fwrite($socket,"User-agent:Opera 10.00\r\n");
fwrite($socket,"Connection:Close\r\n");
fwrite($socket,"\r\n");
 
//теперь передаем данные
fwrite($socket,"$data\r\n");
fwrite($socket,"\r\n");
 
 //теперь читаем ответ
$answer = '';
while(!feof($socket)){
	$answer.= fgets($socket, 4096);
}

//закрываем сокет
fclose($socket);
}


}
// ещё вариант http://www.web-junior.net/otpravka-post-zaprosov-s-pomoshhyu-php-chast-3/
  




