# Fish Feeder Firmware

Algorithm used in the fish feeder device for local fish farmers in Bayan

## Pseudocode
```
Algorithm TriggerTime
    Begin
        While True Do
            // Mengambil waktu dari modul RTC
            current_time = get_time_from_RTC()
            
            // Mengecek apakah user ingin mengubah jam trigger
            If user_wants_to_change_trigger_time() Then
                // Masuk ke state setting
                Enter setting_state
                
                // Masukkan jam trigger baru
                new_trigger_time = input_new_trigger_time()
                
                // Menyimpan 2 waktu trigger
                save_trigger_times(new_trigger_time)
            EndIf
            
            // Mengecek apakah waktu sekarang sama dengan salah satu trigger
            If current_time == trigger_time_1 OR current_time == trigger_time_2 Then
                // Ubah state menjadi on selama beberapa detik
                change_state_to_on_for_some_seconds()
            EndIf
        EndWhile
    End
```
