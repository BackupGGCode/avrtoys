#summary Подключение джойстика Dendy (Nintendo) к Atari
#labels avr,joystick,atari,antic,toys

# Dendy to Atari #

Распиновку брал тут http://xoro.co.ua/2007/10/07/skhema_dzhojjstika_dlja_HSD_2020.html

Протокол примитивный:
  * даём резет.
  * даём восемь тактовых импульсов считывая данные, и получаем инфу о нажатых кнопках.

Подробнее о протоколе см., например, заметку С. Голубева "Ремонт джойстика "Денди" в "Радио", 1996, № 6, с 46

Глянь ещё http://un7ppx.narod.ru/device/dendy.htm ну и собственно мой код ;-) http://avrtoys.googlecode.com/svn/trunk/joystick/
