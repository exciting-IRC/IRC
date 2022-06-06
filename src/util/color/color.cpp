#include "color.hpp"
#include <vector>
namespace color {

inline static color_code create_color(const color_code& code) {
  static color_code begin = "\001\033[", end = "\002m";

  return begin + code + end;
}
// Reference
// https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a

// Regular text

color_code BLK = create_color("0;30");
color_code RED = create_color("0;31");
color_code GRN = create_color("0;32");
color_code YEL = create_color("0;33");
color_code BLU = create_color("0;34");
color_code MAG = create_color("0;35");
color_code CYN = create_color("0;36");
color_code WHT = create_color("0;37");

// Regular bold text
color_code BBLK = create_color("1;30");
color_code BRED = create_color("1;31");
color_code BGRN = create_color("1;32");
color_code BYEL = create_color("1;33");
color_code BBLU = create_color("1;34");
color_code BMAG = create_color("1;35");
color_code BCYN = create_color("1;36");
color_code BWHT = create_color("1;37");

// Regular underline text
color_code UBLK = create_color("4;30");
color_code URED = create_color("4;31");
color_code UGRN = create_color("4;32");
color_code UYEL = create_color("4;33");
color_code UBLU = create_color("4;34");
color_code UMAG = create_color("4;35");
color_code UCYN = create_color("4;36");
color_code UWHT = create_color("4;37");

// Regular background
color_code BLKB = create_color("40");
color_code REDB = create_color("41");
color_code GRNB = create_color("42");
color_code YELB = create_color("43");
color_code BLUB = create_color("44");
color_code MAGB = create_color("45");
color_code CYNB = create_color("46");
color_code WHTB = create_color("47");

// High intensty background
color_code BLKHB = create_color("0;100");
color_code REDHB = create_color("0;101");
color_code GRNHB = create_color("0;102");
color_code YELHB = create_color("0;103");
color_code BLUHB = create_color("0;104");
color_code MAGHB = create_color("0;105");
color_code CYNHB = create_color("0;106");
color_code WHTHB = create_color("0;107");

// High intensty text
color_code HBLK = create_color("0;90");
color_code HRED = create_color("0;91");
color_code HGRN = create_color("0;92");
color_code HYEL = create_color("0;93");
color_code HBLU = create_color("0;94");
color_code HMAG = create_color("0;95");
color_code HCYN = create_color("0;96");
color_code HWHT = create_color("0;97");

// Bold high intensity text
color_code BHBLK = create_color("1;90");
color_code BHRED = create_color("1;91");
color_code BHGRN = create_color("1;92");
color_code BHYEL = create_color("1;93");
color_code BHBLU = create_color("1;94");
color_code BHMAG = create_color("1;95");
color_code BHCYN = create_color("1;96");
color_code BHWHT = create_color("1;97");

// Reset
color_code END = create_color("0");

}  // namespace color
