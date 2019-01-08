###############################
Read Me: Training Data Format
###############################

All signature tracks corresponding to a name/signature-type are saved in a file called <name>.sng.tsv .
Each line of the file represents a single signature track by listing pixel data of the mouse movement captured while drawing the signature on a 500x500 pixel track pad/application window.
The coordinates for a single track are space-separated and coded as follows:

x0 y0 x1 y1 x2 y2 ... xn yn

Beware that any disruption or change in the format will jeopardize the integrity of the signature tracks!