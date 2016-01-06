all:syn_flood ping
syn_flood:syn_flood.cc
	g++ -g $< -o $@
ping:ping.cc
	g++ -g $< -o $@
clean:
	rm -rf syn_flood ping
