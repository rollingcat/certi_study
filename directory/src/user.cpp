#include <stdio.h>

#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999
#define MAX_DIR_NUM 50000
#define MAX_CHILD_NUM 30

struct Dir {
  char name[NAME_MAXLEN + 1];
  Dir* parent;
  Dir* children[MAX_CHILD_NUM];
  int children_num;
  int total_dir;

  void SetName(char* str, int name_len = -1);
  Dir* GetChild(char* str, int name_len);
  void AttachChild(Dir* child);
  void DetachChild(Dir* child);
  void CopyChild(Dir* src);
};

Dir system[MAX_DIR_NUM];
int alloc_idx = 0;

Dir root;
Dir dummy;

// The below commented functions are for your reference. If you want
// to use it, uncomment these functions.
int mstrcmp(const char *a, const char *b);
int mstrncmp(const char *a, const char *b, int len);
int mstrlen(const char *a);
void mstrcpy(char *dest, const char *src);
void mstrncpy(char *dest, const char *src, int len);

Dir* CopyDir(Dir* src);

#define NEW_DIR &system[alloc_idx++];

void Dir::SetName(char* str, int name_len) {
  if (name_len < 0)
    mstrcpy(name, str);
  else
    mstrncpy(name, str, name_len);
}

Dir* Dir::GetChild(char* str, int name_len) {
  for (int i = 0; i < children_num; ++i) {
    if (!mstrncmp(children[i]->name, str, name_len))
      return children[i];
  }
  return 0; // ASSERT
}

void Dir::AttachChild(Dir* new_child) {
  new_child->parent = this;

  children[children_num++] = new_child;
}

void Dir::DetachChild(Dir* dir) {
  int idx = 0;
  for (idx = 0; idx < children_num; ++idx) {
    if (children[idx] == dir)
      break;
  }

  children[idx] = children[--children_num];
//  dummy.sibling = children;
//  Dir* prev = &dummy;
//  Dir* child = children;
//  for (int i = 0; i < children_num; ++i) {
//    if (child == dir) {
//      prev->sibling = child->sibling;
//      break;
//    }
//    prev = child;
//    child = child->sibling;
//  }
//  children = dummy.sibling;
//  --children_num;
}

void Dir::CopyChild(Dir* src) {
  Dir* new_dir = CopyDir(src);
  AttachChild(new_dir);

  for (int i = 0; i < src->children_num; ++i)
    new_dir->CopyChild(src->children[i]);
}


int FindSlash(char* path) {
  int count = 0;
  while (path[count] != '\0') {
    if (path[count] == '/')
      return count;
    ++count;
  }
  return -1;
}

Dir* NewDir(char* str, int name_len) {
  Dir* new_dir = NEW_DIR;
  new_dir->SetName(str, name_len);
  new_dir->children[0] = 0;
  new_dir->children_num = 0;
  new_dir->total_dir = 0;
  return new_dir;
}

Dir* CopyDir(Dir* src) {
  Dir* new_dir = NEW_DIR;
  new_dir->SetName(src->name);
  new_dir->children[0] = 0;
  new_dir->children_num = 0;
  new_dir->total_dir = src->total_dir;
  return new_dir;
}

Dir* Go(Dir* current, char* path) {
  int slash = FindSlash(path);
  if (slash == -1)
    return current;
  return Go(current->GetChild(path, slash), &path[slash + 1]);
}

void Update(Dir* dir, int num) {
  if (dir) {
    dir->total_dir += num;
    Update(dir->parent, num);
  }
}



void init(int n) {
  root.name[0] = '\0';
  root.parent = 0;
  root.children[0] = 0;
  root.children_num = 0;
  root.total_dir = 0;
  alloc_idx = 0;
}

void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1]) {
  Dir* current = Go(&root, &path[1]);
  Dir* new_dir = NewDir(name, -1);
  current->AttachChild(new_dir);
  Update(new_dir->parent, 1);
}

void cmd_rm(char path[PATH_MAXLEN + 1]) {
  Dir* current = Go(&root, &path[1]);
  current->parent->DetachChild(current);
  Update(current->parent, -(current->total_dir + 1));
}

void cmd_cp(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {
  Dir* src = Go(&root, &srcPath[1]);
  Dir* dst = Go(&root, &dstPath[1]);

  dst->CopyChild(src);
  Update(dst, src->total_dir + 1);
}

void cmd_mv(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {
  Dir* src = Go(&root, &srcPath[1]);
  Dir* dst = Go(&root, &dstPath[1]);

  src->parent->DetachChild(src);
  Update(src->parent, -(src->total_dir + 1));

  dst->AttachChild(src);
  Update(dst, src->total_dir + 1);
}

int cmd_find(char path[PATH_MAXLEN + 1]) {
  static const char* debug = "/cxyvef/zg/nk/fgls/";
  if (!mstrcmp(debug, path))
    printf("%s\n", path);

  Dir* current = Go(&root, &path[1]);
  return current->total_dir;
}


int mstrcmp(const char *a, const char *b)
{
  int i;
  for (i = 0; a[i] != '\0'; i++)
  {
    if (a[i] != b[i])
      return a[i] - b[i];
  }
  return a[i] - b[i];
}

int mstrncmp(const char *a, const char *b, int len)
{
  for (int i = 0; i < len; i++)
  {
    if (a[i] != b[i])
      return a[i] - b[i];
  }
  return 0;
}

int mstrlen(const char *a)
{
  int len = 0;

  while (a[len] != '\0')
    len++;

  return len;
}

void mstrcpy(char *dest, const char *src)
{
  int i = 0;
  while (src[i] != '\0')
  {
    dest[i] = src[i];
    i++;
  }
  dest[i] = src[i];
}

void mstrncpy(char *dest, const char *src, int len)
{
  for (int i = 0; i<len; i++)
  {
    dest[i] = src[i];
  }
  dest[len] = '\0';
}
