clear
#sh ./clearImages
rm a.out
rm main.exe
#gcc -pthread main.c -lm
gcc -g -rdynamic -pthread -Wformat=1 ./simulator/main.c -lm
cp a.out main.exe
#python ./monitoring_ui/main_mapWindow.py &
#python ./monitoring_ui/main_statsWindow.py &

#Before you run the tests you'll want to backup the worlds
#mv -f ./outputs/world_agents_a.txt ./outputs/world_agents_bkp_a.txt
#mv -f ./outputs/world_agents_b.txt ./outputs/world_agents_bkp_b.txt
#./main.exe -t
#mv -f ./outputs/world_agents_bkp_a.txt ./outputs/world_agents_a.txt
#mv -f ./outputs/world_agents_bkp_b.txt ./outputs/world_agents_b.txt
# START/SEED a new simulation
nohup nice ./main.exe -s > /media/user/RaidDrive/CharliesStuff/production/AIWorld6/out.txt 2> /media/user/RaidDrive/CharliesStuff/production/AIWorld6/err.txt &
# CONTINUE an existing simulation
#./main.exe -c
# FIGHT between two worlds
#./main.exe -f
# TEST the code
#./main.exe -t
#sh ./buildImages
