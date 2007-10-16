for %%f in (*.wav) do "../HCopy.exe" -F WAV %%f %%f_htk.wav
for %%f in (*_htk.wav) do "../HCopy.exe" -C "../config_hcopy.txt" %%f %%f.mfc
pause