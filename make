clear
sh ./clearImages
rm a.out
rm main.exe
#gcc -pthread main.c -lm
gcc -g -rdynamic -pthread -Wformat=1 ./simulator/main.c -lm
cp a.out main.exe
python ./monitoring_ui/main.py &
#Before you run the tests you'll want to backup the worlds
#mv -f ./outputs/world_agents_a.txt ./outputs/world_agents_bkp_a.txt
#mv -f ./outputs/world_agents_b.txt ./outputs/world_agents_bkp_b.txt
#./main.exe -t
#mv -f ./outputs/world_agents_bkp_a.txt ./outputs/world_agents_a.txt
#mv -f ./outputs/world_agents_bkp_b.txt ./outputs/world_agents_b.txt
# START/SEED a new simulation
./main.exe -s
# CONTINUE an existing simulation
#./main.exe -c
# FIGHT between two worlds
#./main.exe -f
#sh ./buildImages
