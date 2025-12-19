import wave
import os
import numpy as np


"D:\Git\Buckshot_Roulette_Arduino\Tools\Sound_AlienShooter1_22050_16\music\end.ogg"
"D:\Git\Buckshot_Roulette_Arduino\Tools\Sound_AlienShooter1_22050_16\music\menu_mus.ogg"

list_wav_path = [
    # (input path, C header name, C array name)
    ########## Sound #########
    # (r"..\Sound_AlienShooter1_22050_16\shotgun.wav", "shotgun_sound", "shotgun_sound"),
    # (r"..\Sound_AlienShooter1_22050_16\ammo_pickup.wav", "load_shell_sound", "load_shell_sound"),
    # (r"..\Sound_AlienShooter1_22050_16\drop_shotgunshell.wav", "shotgun_drop_sound", "shotgun_drop_sound"),
    # (r"..\Sound_AlienShooter1_22050_16\mouse_over.wav", "pick_sound", "pick_sound"),
    # (r"..\Sound_AlienShooter1_22050_16\mouse_click.wav", "assign_sound", "assign_sound"),
    # (r"..\Sound_AlienShooter1_22050_16\armor_hit1.wav", "hit_sound", "hit_sound"),
    # (r"..\Sound_AlienShooter1_22050_16\weapon_pickup.wav", "weapon_pickup_sound", "weapon_pickup_sound"),
    # (r"..\Sound_AlienShooter1_22050_16\broke_armor.wav", "break_sound", "break_sound"),
    # (r"..\Sound_AlienShooter1_22050_16\take_healh.wav", "health_sound", "health_sound"),
    # (r"..\Sound_AlienShooter1_22050_16\gib_drop.wav", "adrenaline_sound", "adrenaline_sound"),
    # (r"..\Sound_AlienShooter1_22050_16\null.wav", "null_sound", "null_sound"),
    ########## Music #########
]

sample_rate = 22050

def convert_wav_to_c_array(wav_file_path, output_c_header_path, array_name, target_samplerate):
    # Convert the WAV file to an 8-bit C-array.
    # Note: 8-bit PCM is usually UNSIGN (0-255), but if the original data is SIGNED
    # (from -128 to 127), adjustment is needed. We will convert it back to 0-255 as the default.

    try:
        print(wav_file_path)

        with wave.open(wav_file_path, 'rb') as wf:
            # Get WAV info
            channels = wf.getnchannels()
            sample_width = wf.getsampwidth()
            frame_rate = wf.getframerate()
            n_frames = wf.getnframes()

            print(f"File: {wav_file_path}")
            print(f"Chanel: {channels}, Bit depth: {sample_width*8}-bit, Frequency: {frame_rate} Hz")

            if channels > 1:
                print("ERROR: The WAV file must be Mono (1 channel).")
                return

            if frame_rate != target_samplerate:
                print(f"WARNING: Sampling frequency ({frame_rate} Hz) does not match the target ({target_samplerate} Hz).")

            # Read data frame
            raw_frames = wf.readframes(n_frames)

            if sample_width == 1:
                audio_data = np.frombuffer(raw_frames, dtype=np.uint8)

            elif sample_width == 2:
                audio_data = np.frombuffer(raw_frames, dtype=np.int16)
                
                # CONVERTING 16-bit (SIGNED) to 8-bit (UNSIGNED)
                # a) Reduce bit depth: Divide by 256 (equivalent to 2^8) to convert 16-bit to 8-bit
                # b) Convert SIGNED (-128 to 127) to UNSIGNED (0-255): Add 128

                # Divide by 256 (or shift right by 8 bits)
                audio_8bit_signed = audio_data // 256

                # Change from (-128...127) to (0...255)
                audio_data = (audio_8bit_signed + 128).astype(np.uint8)

            else:
                print(f"ERROR: Sample depth {sample_width*8}-bit not supported (8-bit and 16-bit only).")
                return

            # Create C header
            c_array_str = f"// File: {os.path.basename(wav_file_path)}\n"
            c_array_str += f"// Chanel: Mono, Bit depth: 8-bit PCM\n"
            c_array_str += f"// Sampling frequency: {frame_rate} Hz\n"
            c_array_str += f"// Size: {len(audio_data)} bytes\n\n"

            c_array_str += f"const unsigned char {array_name}[{len(audio_data)}] PROGMEM = {{\n    "

            # Format the values ​​with commas, 16 values ​​per line
            values = [str(x) for x in audio_data]
            c_array_str += ",\n    ".join([", ".join(values[i:i+16]) for i in range(0, len(values), 16)])

            c_array_str += "\n};\n"

            # Write to C header
            with open(output_c_header_path, 'w', encoding="utf-8") as f:
                f.write(c_array_str)

            print(f"SUCCESS: Created {output_c_header_path} with {len(audio_data)} sample 8-bit.")

            return True
    except FileNotFoundError:
        print(f"ERROR: File not found {wav_file_path}")
    except Exception as e:
        print(f"PROCESSING ERROR: {e}")

    return False

if __name__ == "__main__":
    count = 0
    for path in list_wav_path:
        if convert_wav_to_c_array(path[0], f"output/{path[1]}.h", path[2], sample_rate):
            count += 1

    if count == len(list_wav_path):
        print("All done!")
