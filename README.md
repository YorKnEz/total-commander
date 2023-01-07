# About file explorer

File explorer is a multi-panel file explorer app, that allows all basic file operations and navigation between computer locations. Our app is designed with the aim to provide the best means of file navigation and ease of use.

## Installation

// to do

## Shortcuts

### File selection

`Ctrl + A` - Select all files\
`Ctrl + D` - Deselect all files\
`Ctrl + I` - Invert selection

### File manipulation

`Ctrl + C` - Copy all selected files\
`Ctrl + V` - Paste the copied files\
`Ctrl + X` - Cut all selected files\
`Enter` - Open last selected file\
`Delete` - Delete last selected file\
`F2` - Rename all selected files\
`F3` - Make directory\
`F4` - Copy all selected files to a path\
`F5` - Delete all selected files\
`F6` - Move all selected files to a path

### File sorting

`Shift + F2` - Sort by name\
`Shift + F3` - Sort by extension\
`Shift + F4` - Sort by size\
`Shift + F5` - Sort by date

### View

`Ctrl + R` - Refresh explorer\
`Ctrl + T` - Switch to the next theme\
`Ctrl + SHIFT + T` - Switch to the previous theme\
`F1` - Open this page

### Navigation

`Backspace` - Go back one folder\
`Tab` - Switch focus to next explorer\
`Shift + Tab` - Switch focus to previous explorer\
`Up` - Move up one file / Scroll up\
`Down` - Move down one file / Scroll down

## Customizing

You can create your own theme if you don't like the default ones, for that we have `themes-template.txt`, where you can see the format of the themes and an example. After you created your theme, you just need to add it at the end of `themes.txt` (that's where all the themes should be saved).

### Note: modifying the code

The best part about our project is that it is open source, that means you can take a look at the code and modify it. You can change literally anything you want (at your own risk).

If you are willing to modify the code yourself, you will need to compile it afterwards, so here are the compilation commands:

Linux: `g++ src/*.cpp src/components/*.cpp -Iinclude -Iinclude/components -std=c++20 -lsfml-graphics -lsfml-window -lsfml-system -o total-commander-linux && ./total-commander-linux && clear`

Windows: `g++ src/*.cpp src/components/*.cpp -Iinclude -Iinclude/components -std=c++20 -IPATH_TO_SFML_INCLUDE -DSFML_STATIC -LPATH_TO_SFML_LIB -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -o total-commander && total-commander.exe`

For this to work, you will need to install [SFML](https://www.sfml-dev.org/download/sfml/2.5.1/).

## Credits

Default icons taken from the following sources: [1](https://www.flaticon.com/packs/files-69), [2](https://www.flaticon.com/packs/cloud-network-5), [3](https://www.flaticon.com/packs/diagram), [4](https://www.flaticon.com/packs/arrows-85).


