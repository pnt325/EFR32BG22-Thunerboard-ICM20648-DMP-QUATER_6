"C:\Program Files (x86)\Atmel\Studio\7.0\AtmelStudio.exe" EMD-Core\EMD-Core-ICM20648.cproj /rebuild release /out scripts\ICM20648-EMDCore.txt
"C:\Program Files (x86)\Atmel\Studio\7.0\AtmelStudio.exe" EMD-App\EMD-App-ICM20648.cproj /rebuild release /out scripts\ICM20648-EMDApp.txt
mkdir release\ICM20648
copy EMD-App\Release\EMD-App-ICM20648.bin release\ICM20648
copy EMD-App\Release\EMD-App-ICM20648.elf release\ICM20648