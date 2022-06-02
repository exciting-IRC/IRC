#include <rapidcheck.h>

#include <algorithm>
#include <vector>

#if defined(TEST)
# define TEST_MAIN main
#endif

int TEST_MAIN() {
  rc::check("double reversal yields the original value",
            [](const std::vector<int> &l0) {
              auto l1 = l0;
              std::reverse(begin(l1), end(l1));
              std::reverse(begin(l1), end(l1));
              RC_ASSERT(l0 == l1);
            });

  int flag = 0;

  return 0;
}
