<?php

// Показать всю информацию, по умолчанию INFO_ALL
phpinfo();

/*
INFO_GENERAL	1	Строка конфигурации, размещение php.ini, дата построения/build, Web-сервер, система и т.д.
INFO_CREDITS	2	Кредиты PHP 4. См. также phpcredits().
INFO_CONFIGURATION	4	Текущие Local и Master значения php-директив. См. также ini_get().
INFO_MODULES	8	Загруженные модули и их соответствующие настройки.
INFO_ENVIRONMENT	16	Environment Variable информация, доступная также в $_ENV.
INFO_VARIABLES	32	Показывает все предопределённые переменные из EGPCS (Environment, GET, POST, Cookie, Server).
INFO_LICENSE	64	Информация PHP License. См. также license faq.
INFO_ALL	-1	Всё выше указанное. Это значение по умолчанию.
*/