# code style

this is a quick run-down of basic code style guidelines. code is expected to be
formatted when a pull request is submitted.

## general rules

### style

all of these can automatically be corrected using `make format`

- indent using tab
- tab display width (should) be equal to 2 spaces
- symbols in snake case
- file names in snake case
- c++ classes are in CamelCase
- open brackets on same line

### naming

most (but not all) naming convention errors can be corrected using `make
format` too, but might cause incompatibility issues. make sure to commit code
before formatting as a failsafe.

- prefix symbols with `hh_` (e.g. `hh_game_loop_main`)
- prefix global variables with `g_hh_` (e.g. `g_hh_example_global_variable`)
- global constants are uppercase snake (e.g. `HH_SERIAL_BAUD`)
- enum typedefs are prefixed with `hh_e_` (e.g. `hh_e_entity_state`)
- struct typedefs are prefixed with `hh_s_` (e.g. `hh_s_bam_tile`)
- custom typedefs are prefixed with `hh_` and suffixed with `_t` (e.g. `hh_bam_tile_t`)
- library hooks that need specific symbol names are exempt from the naming
  conventions (e.g. `main` or `HAL_UART_MspInit`)
- names are always in English
- filenames should not be prefixed by anything.

## others

- document **how to use** code using doxygen-style comments in headers
- document **what code is doing** using inline comments (where applicable)
- don't write redundant comments (e.g. `int c = a + b; // add a and b`)
- comments are always in English

### markdown

- indent using spaces, align to text start

### vhdl

- vivado puts new files in subfolders, all files (source files, constraints,
  etc.) should be directly in the basys3/basys3.srcs/ folder
- use snake case for naming components
- use uppercase snake case for naming signals and I/O lines
- use lower case keywords
- testbench name is the component name with `_tb` as suffix
- vhdl filename is the same as the component name
- vhdl files should end in the `.vhd` file extension, not `.vhdl`
- use spaces around the colon used for setting the type of signal definitions
- do not mix uppercase and lowercase in entity/component/port/signal names
- remove the default vivado file headers (large comment blocks with file info)
- add a comment to the entity definition describing what the entity does
- library import names should be lowercase
- with the exception of off by one shifts, absolutely NO magic numbers are
  allowed. use existing constants if possible, or create new ones in a consts
  file instead of using magic numbers.
- entities/components without a generic map should put the opening `port (` on
  the same line as the `entity ... is ` line
- entities/components with a generic map should be formatted like this:
  ```
  entity ... is
    generic map(
      NAME : type := default;
      NAME : type := default)
    port map(
      NAME : in type;
      NAME : out type);
  end ...;
  ```
- port/generic maps with only one signal should be condensed into a single line
- all library declarations should be grouped together at the top of the file
- all `use` directives should be grouped together below the library
  declarations
- use a space before and after the comment syntax, e.g. `variable x : integer
  := 0; -- comment here`
