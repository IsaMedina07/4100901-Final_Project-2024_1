# Control de Iluminación Automático.

Con la finalidad de aplicar los conceptos vistos en el transcurso del semestre, se hizo la implementación del prototipo de un sistema de "iluminado inteligente" para un hogar. 
A través de la núcleo STMl476rg y del uso del módulo WiFi se programaron los leds correspondientes para tener control tanto directo como remoto. 

## Funcionamiento STM32 L476rg y módulo WiFi.
La programacion para el embebido consiste de una serie de funciones que se encargan del control del encendido y apagado de los LED correspondientes a cada habitación de una casa, teniendo uno para la habitación, la sala de estar, la cocina y el baño. Cada uno de ellos está controlado por, primero, un teclado hexadecimal y, segundo, por una 
aplicación para celular conectada vía Wifi. 

### Funciones Específicas.

### - Funciones en la Núcleo STM32l476rg: 
La STM32 está programada de forma tal que se solicita una clave para el ingreso y manipulación de las luces. Cuando la clave es correcta, se activa el sistema 
para poder controlar la iluminación. Desde el teclado hexadecimal se pueden activar las luces presionando la tecla adecuada. 
Si una de las habitaciones permanece mucho tiempo encendida, se apaga automaticamente después de cinco segundos o del tiempo deseado. Si se quiere apagar manualmente, basta con oprimir dos veces la tecla correspondiente.
Si el sistema detecta una clave incorrecta, se activa un mensaje de alerta y las luces indicarán parpadeando que se trata de un "intruso". Todas las acciones quedan
registradas en la pantalla OLED y la comunicación por UART en el YAT. 

### - Funciones para el módulo WiFi:
A través de una aplicación en donde se pueden ingresar dos usuarios es posible encender o apagar a gusto las luces del hogar, ya que los datos de usuario y de control de las luces se encuentran almacenados en un sevidor de FireBase. El ESP01 tiene comunicacion con la núcleo STm32l476rg a través del periférico RTC del STM32L476RG, donde es posible compartir información por medio del puerto de comunicación USART.
