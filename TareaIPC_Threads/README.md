Programa de IPC con muchos threads.

Para ejecutar el programa se necesitan 5 terminales abiertos.
En cualquier terminal tipear "make" y luego presionar "Enter".
Luego en el Terminal 1 ingresar "./receive_s.exe".
En el Terminal 2 ingresar "./send_s.exe", después de ingresar esto se nos pide el PID de "receive_s.exe",
para esto en el Terminal 3 se debe ingresar "ps -A | grep receive_s.exe", luego debemos ingresar
el primer número que nos aparece en esa lista (PID) en el Terminal 2.
Después en el Terminal 3 debemos ingresar "./receive_p.exe".
Y finalmente, en el Terminal 4 se ingresa "./read_sh_m.exe",
y en el Terminal 5 se ingresa "./receive_m_q.exe"

Para terminar los programas, solo debe presionar "ctrl + c", ya que estos todos estos programas
están en un bucle infinito.