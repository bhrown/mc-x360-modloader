nnoremap @1 :tabnew<cr>:term rm build/*; cmd.exe /c build.bat; cp build/module.xex game/module.xex<cr>
nnoremap @2 :tabnew<cr>:term /mnt/c/Users/asdf/Desktop/xenia_canary_windows/xenia_canary.exe game/extract.xex --debug<cr>
nnoremap @3 :tabnew<cr>:term /mnt/c/Users/asdf/Desktop/xenia_canary_windows/xenia_canary.exe build/loader.xex --debug & tail -f /mnt/c/Users/asdf/Desktop/xenia_canary_windows/xenia.log<cr>
nnoremap @4 :tabnew<cr>:term /mnt/c/Users/asdf/Desktop/xenia_canary_windows/xenia_canary.exe game/extract.xex --debug & tail -f /mnt/c/Users/asdf/Desktop/xenia_canary_windows/xenia.log<cr>
