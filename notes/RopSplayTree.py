class Node(object):
    """Node object for SplayTree.
    """
    def __init__(self, ch: str, left: 'Node' = None, right: 'Node' = None, parent: 'Node' = None) -> None:
        """Instantiates Node object for SplayTree.
        Parent, left and right are pointers to parent and child nodes.
        The size is initialized at 1, adjusted with insert and delete to represent
        the count of the nodes in the subtree.
        """
        self.ch = ch
        self.left = left
        self.right = right
        self.parent = parent
        self.size = 1


class RopeTree(object):
    """SplayTree/Self-Adjusting Binary Search Trees.
    """

    def __init__(self, init_str: str):
        """Tree is represented by its root node, initially None.
         The following methods are implemented:
             Create a new splay tree.
             The string init_str is used to populate the tree.
             Each node of the tree contains one letter:
                 tree = RopeTree(init_str)
             Print the string contained in the tree:
                 tree.print()
             Cut a piece of string and rearrange it to another place:
                 tree.new_order(i, j, k)
     """
        self.root = None

        if init_str:
            self.root = Node(init_str[0])
            for i in range(1, len(init_str)):
                self.root = Node(init_str[i], self.root, None)
                self._set_parent(self.root.left, self.root)
                self._count_size(self.root)
            self._balance()

    def _set_parent(self, child: 'Node', parent: 'Node') -> None:
        """Sets the child's parent to node "parent".
        :return:
        """
        if child:
            child.parent = parent

    def _keep_parent(self, node: 'Node') -> None:
        """Sets the parent for the node's children.
        :return:
        """
        self._set_parent(node.left, node)
        self._set_parent(node.right, node)

    def _count_size(self, node: 'Node') -> int:
        """Finds count of the nodes in the subtree rooted "node".
        :param node: Node
        :return: Count of the nodes in the subtree.
        """
        node.size = 1
        if node.left:
            node.size += node.left.size
        if node.right:
            node.size += node.right.size

    def _get_node_id(self, node: 'Node') -> int:
        """Gets the ordinal number of the node when in_order traversing.
        :param node: Node
        :return: Ordinal number of the node.
        """
        return node.size - 1 - (node.right.size if node.right else 0)

    def _rotate(self, parent: 'Node', child: 'Node') -> None:
        """Rotates the vertices.
        With one turn, you can swap the parent with the child,
        as shown in the figure below.
        :param parent: The parent node that should become a child.
        :param child: The child node that should become a parent.
        :return:
        """
        """    z            y
              / \          / \
             y   c   <->  a   z
            / \              / \
           a   b            b   c   
        """
        gparent = parent.parent
        if gparent:
            if gparent.left == parent:
                gparent.left = child
            else:
                gparent.right = child

        if parent.left == child:
            parent.left, child.right = child.right, parent
        else:
            parent.right, child.left = child.left, parent

        self._keep_parent(parent)
        self._keep_parent(child)
        child.parent = gparent
        self._count_size(parent)
        self._count_size(child)

    def _splay(self, node: 'Node') -> 'Node':
        """Raises a node up and makes it a root.
        When a node is moved up, the distance to the root is reduced not only
        for the node being lifted, but also for all its childs in the current subtrees.
        For this, the technique of zig-zig and zig-zag turns is used.
        :param node: A node that should be lifted up and become the root.
        :return: A new tree root.
        """
        while node.parent:
            parent = node.parent
            gparent = parent.parent
            if not gparent:
                self._rotate(parent, node)
                self.root = node
                return node
            else:
                zigzig = (gparent.left == parent) == (parent.left == node)
                if zigzig:
                    self._rotate(gparent, parent)
                    self._rotate(parent, node)
                else:
                    self._rotate(parent, node)
                    self._rotate(gparent, node)
        self.root = node
        return node

    def _balance(self) -> None:
        """To balance the tree, we call the find for the middle node.
        :return:
        """
        self._find(self.root, (self.root.size - 1) // 2)

    def _find(self, node: 'Node', id: int) -> 'Node':
        """Finds and returns node with given id, else, returns None.
        If a node is found, then it is lifted up and becomes the root.
        :param node: The root of the tree in which the node is searched.
        :param id: The id of the node to be found.
        :return: A node with given id if such a node exists, else, None.
        """
        while node:
            node_id = self._get_node_id(node)

            if id == node_id:
                return self._splay(node)
            elif id < node_id and node.left:
                node = node.left
            elif id > node_id and node.right:
                id -= node_id + 1
                node = node.right
            else:
                return self._splay(node)
        return None

    def _split(self, node: 'Node', id: int) -> ('Node', 'Node'):
        """Divides the tree into two.
        In one tree, all values are to the left of the node with the identifier id,
        and in the other, all values are to the right.
        :param node: The tree identified by the root "node" to be split.
        :param id: Id of the node on which the split will be performed.
        :return: (left, right). Roots of left and right subtrees.
        """
        if not node:
            return None, None

        node = self._find(node, id)
        if node:
            node_id = self._get_node_id(node)

            if node_id == id:
                left = node.left
                if left:
                    node.left, left.parent = None, None
                    node.size -= left.size
                return left, node
            if node_id < id:
                return node, None
        return None, None

    def _merge(self, left: 'Node', right: 'Node') -> 'Node':
        """Merges 2 subtrees into one.
        Take the node with id 0 of the right tree and drag it up.
        After that, add the left tree as the left subtree.
        :param left: The root of the left subtree to be merged.
        :param right: The root of the right subtree to be merged.
        :return: The root of the new subtree.
        """
        if not left:
            return right
        if not right:
            return self._find(left, 0)

        right = self._find(right, 0)
        right.left, left.parent = left, right
        right.size += left.size
        node = self._find(right, 0)
        return node

    def print(self) -> None:
        """User interface for printing the string contained in the tree.
        Use in-order traversal of a binary tree.
        :return:
        """
        cur_node = self.root
        stack = []
        while True:
            if cur_node:
                stack.append(cur_node)
                cur_node = cur_node.left
            elif len(stack) > 0:
                cur_node = stack.pop()
                print(cur_node.ch, end='')
                cur_node = cur_node.right
            else:
                break

    def new_order(self, i, j, k) -> None:
        """User interface for cut a piece of string and rearrange it to another place.
        Cut the substring S[i..j] (where i and j are indexed from 0)
        and insert it after the k-th character of the remaining string
        (where k is indexed from 1).
        If k = 0, then insert the cut piece at the beginning .
        :param i: Index of the beginning of the string to be cut.
        :param j: Index of the end of the string to be cut.
        :param k: Index of the letter after which the piece should be inserted.
        :return:
        """
        left, right = self._split(self.root, i)
        cut, right = self._split(right, j - i + 1)
        root = self._merge(left, right)
        left, right = self._split(root, k)
        root = self._merge(cut, right)
        self.root = self._merge(left, root)
        self._balance()

def main():

    # tree initialization
    tree = RopeTree(input())

    # read the number of requests
    n = int(input())

    # read requests:
    # cut a piece of string and rearrange it to another place
    for __ in range(n):
        i, j, k = [int(i) for i in input().split()]
        tree.new_order(i, j, k)

    # print the word contained in the tree
    tree.print()


if __name__ == "__main__":
    main()