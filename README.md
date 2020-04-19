# GenBackgroundEffectsDemo
Recreation of battle background effects from the game EarthBound on the Sega Genesis.

# Instructions
Press up or down on the controller to cycle through different effects.

# About This Demo
I started this demo trying to figure out if the effects described in [Retro Game Mechanics Explained](https://www.youtube.com/channel/UCwRqWnW5ZkVaP_lZF7caZ-g)'s video on the [battle backgrounds in the game Earthbound](https://www.youtube.com/watch?v=zjQik7uwLIQ).  My goal was to recreate these effects on the Sega Genesis.
## Effects
The effects described in the EarthBound video are:
1. Palette Cycling
2. Scrolling
3. Horizontal Oscillation
4. Interleaved Horizontal Oscillation
5. Vertical Oscillation
6. Transparency

Before starting this project, I knew that effects 1-4 were possible on the Genesis.  The original project was a Vertical Oscillation proof-of-concept, and later expanded to include the other effects (except Transparency).

# Implementation
I recommend watching the [EarthBound Battle Backgrounds video](https://youtu.be/zjQik7uwLIQ?t=143) for a good visual explanation of how the oscillation effects work.  On the SNES, Horizontal Direct Memory Access (HDMA) is used to change the background's horizontal and vertical scroll positions as the background is being drawn to the screen.  Since the Genesis does not support HDMA, other mechanisms must be used to achieve the same effect.

For the three oscillation effects, the demo calculates 224 offset values per frame, one for each line drawn to the screen.  In Horizontal and Interleaved Horizontal Oscillation, per-line plane scrolling is used to offset each line by these values.

For Vertical Oscillation, horizontal interrupts are used to change the plane's vertical scroll value every 2 lines (every line is currently too slow).  Changing the scroll value while the plane is being drawn to the screen causes a "rippling" effect.  If the plane shifts down, lines will be repeated, stretching them out.  If the plane shifts up, lines will be skipped, making the image appear more compressed.  Jon Burton's video on the [Sonic 3D Blast intro](https://www.youtube.com/watch?v=IehwV2K60r8) gave me an idea how this could be done, as that game stretches an 80-pixel image to fill the 224 lines of the screen.

# Formula
The mathematical formula used to generate these backgrounds, which I found [here](https://forum.starmen.net/forum/General/Tech/Help-with-allegro-and-c/page/1#post1131426) is:
```offset(y, t) = A * sin (F * y + S * t)```

| Variable | Explanation |
| ------------- | ------------- |
| offset(y, t) | How many pixels the plane will be offset horizontally/vertically for each line **y** and frame number **t**. |
| A | Amplitude, the maximum number of pixels the plane will be offset from center. |
| F | Frequency, affects how quickly the sine values will repeat within a given frame. |
| y | Current line for which the offset is being calculated.  For each frame, y starts at 223 and counts down to 0. |
| S | Speed or "time shifting", affects how quickly the sine values will repeat over time. |
| t | Current frame number.  Starts at 0 and increases by 1 every frame. |

The [Sine function](https://en.wikipedia.org/wiki/Sine) is periodic; its results repeat for every multiple of 2π.  SGDK's sinFix16() implementation takes inputs from 0...1024 to represent 0...2π and returns values between -1 and 1 in fix16 format (10 bits used for the whole number and 6 bits for the decimal).  Our input to the sine function, ```(F * y + S * t)``` determines how quickly we reach a multiple of 1024, and therefore how quickly the results will repeat.

## Optimization
The three multiplications in the original formula make it too slow to run on the Genesis.  Over time I optimized the formula in four stages, as follows:
1. ```S * t``` does not change while the frame is being drawn, so it can be calculated once before we start the 224-line loop.
2. Since y decreases by 1 on each line, the value of ```F * y``` differs from one line to the next by -F.  We can calculate a **sine_table_idx** for line 223, ```F * 223 + S * t```, and for each subsequent line, we decrease this value by **F**.
3. Following similar thinking to #2: Since t increases by 1 for each frame, for the first line of each frame (line 223), the value of sine_table_idx differs by +S.  We can calculate an **initial_sine_table_idx** before the first frame, ```F * 223 + S * 0``` and for each subsequent frame, we increase this initial value by **S**.  We then set sine_table_idx to this value at the start of each frame.
4. By only using amplitudes which are powers of 2, we can use bit-shifting instead of multiplication.  The amplitude values stored in the code are the exponents of 2.  Technically, left-shifting a negative int is undefined behavior, but the way GCC handles it appears to work.

These optimizations assume **A**, **F**, and **S** do not change while a given background is being displayed.

If you have other ideas how the code can be improved, let me know!

# Photosensitivity Warning
When creating or modifying backgrounds, it is very easy to accidentally cause flickering and strobing effects.  These could potentially trigger seizures for people with photosensitive epilepsy.  Please use appropriate caution and discretion.
