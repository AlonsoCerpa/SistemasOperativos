all: send_s.exe receive_s.exe receive_p.exe read_sh_m.exe receive_m_q.exe

send_s.exe:
	g++ send_signal.cpp -o send_s.exe -pthread

receive_s.exe:
	g++ receive_signal.cpp -o receive_s.exe

receive_p.exe:
	g++ receive_pipe.cpp -o receive_p.exe -pthread

read_sh_m.exe:
	g++ read_sh_mem.cpp -o read_sh_m.exe

receive_m_q.exe:
	g++ receive_msg_que.cpp -o receive_m_q.exe 

clean:
	rm -f send_s.exe receive_s.exe receive_p.exe read_sh_m.exe receive_m_q.exe