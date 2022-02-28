#ifndef Matrice2D_H
#define Matrice2D_H

#include <algorithm> // std::swap
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t
#include <ostream> // std::ostream
#include <iostream> // std::cout

/**
  Classe che implementa una matrice 3D di dati generici di tipo T.
	La trasformazione è effettuata utilizzando un funtore di trasformazione F.
	L'uguaglianza tra due dati di tipo T è fatta usando un funtore di
	uguaglianza E.
   
  @brief Matrice2D di tipo T

  @param T tipo di dato
*/

template <typename T>
class Matrice2D {

  struct element {
    T _value; ///< Valore del nodo
    const unsigned int _row; ///< Posizione riga del nodo
    const unsigned int _column; ///< Posizione colonna del nodo
    
    /**
    Costruttore che inizializza un elemento della Matrice2D
          
    @param v valore dell elemento
    @param x posizione riga dell elemento
    @param y posizione colonna dell elemento
    */

    element(const T &v, unsigned int x, unsigned int y) : _value(v), _row(x), _column(y) {}

	}; ///< Fine struttura element

  /**
  Operatore di output per la struct elemento

  @param os oggetto di stream output
  @param e elemento da stampare
  @return reference all output stream
  */
  
  friend std::ostream &operator<<(std::ostream &os, const element &elem) {
    os << "Alla posizione : [" << elem._row << ","
       << " " << elem._column << "]"
       << " si trova il valore : " << elem._value << std::endl;
    return os;
  }

  /**
  * Struttura che rappresenta un nodo della lista
  */
  struct node {
    node *next;
    element ele;
        
    // Costruttore di default
    node() : next(nullptr) {}

    /** Costruttore che inizilizza un nodo
    * @param v valore dell'elemento da inserire
    * @param x posizione riga dell'elemento
    * @param y posizione colonna dell'elemento
    */
    node(const T &v, unsigned int x, unsigned int y) : next(nullptr), ele(v,x,y) {};

    /**
    * Distruttore
    */
    ~node() { next = nullptr; }
  }; // Fine struttura nodo

private : 
  node *_head;
  int _totalSize;
  int _rows;
  int _columns;

public:

  /**
  * Costruttore che inizializza la Matrice 3D
  * @param rows numero di righe delle matrice
  * @param columns numero di colonne della matrice
  */

  Matrice2D(unsigned int rows, unsigned int columns) : _head(nullptr), _rows(rows), _columns(columns), _totalSize(rows*columns)  {};

  /**
  * Costruttore di copia quando la matrice è dello stesso tipo
  * @param other matrice da copiare 
  * @throw eccezzione di allocazione di memoria
  */

  Matrice2D(const Matrice2D &other) : _head(nullptr), _rows(other._rows), _columns(other._columns), _totalSize(other._totalSize) {
    node *curr = other._head;

    try {
      while (curr != nullptr) {
        add(curr->ele._value, curr->ele._row, curr->ele._column);
        curr = curr->next;
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  /**
  Costruttore di copia, usato quando la lista da copiare è di tipo diverso dalla
  lista da creare.

  @param other lista da copiare
  @throw eccezione di allocazione di memoria
  */
  template <typename U>
  explicit Matrice2D(const Matrice2D<U> &other) : _head(nullptr), _rows(other.getRows()), _columns(other.getColumns()), _totalSize(other.getTotalSize()) {
    typename Matrice2D<U>::const_iterator ib, ie;
    ib = other.begin();
    ie = other.end();

    try {
      while (ib != ie) {
        add(static_cast<T>((*ib)._value), (*ib)._row, (*ib)._column);
        ++ib;
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  /**
  Operatore di assegnamento
     
  @param other lista da copiare
  @return reference a this
  @throw eccezione di allocazione di memoria
  */

  Matrice2D &operator=(const Matrice2D &other) {
    if (this != &other) {
      Matrice2D tmp(other);
      std::swap(_head, tmp._head);
      std::swap(_totalSize, tmp._totalSize);
      std::swap(_rows, tmp._rows);
      std::swap(_columns, tmp._columns);
    }
    return *this;
  }

  ~Matrice2D() { clear(); }

  /**
  * Get per i valori della Matrice2D
  */
  int getRows() const { return _rows; }
  int getColumns() const { return _columns; }
  int getTotalSize() const { return _totalSize; }


  /**
    * Cancella il contenuto nella lista
  */
  void clear() {
    node *curr = _head;
    node *tmp = nullptr;
    
    while (curr != nullptr) {
      tmp = curr->next;
      delete curr;
      curr = tmp;
    }
    
    _totalSize = 0;
    _head = nullptr;
  }

  void add(const T &value, int row, int column) {
    
    node *nNode = new node(value, row, column);

    if((nNode->ele._row > this->_rows) || (nNode->ele._column > this->_columns)) {
      std::cout << "Non posso aggiungere il valore " << value << ", out of bounds" << std::endl;
        delete(nNode);
        return;
    }
      
    if(_head == nullptr) {
      _head = nNode;
      return;
    }
        
    node *curr = _head;
    node *prev = _head;

    while(curr != nullptr){
      prev = curr;
      if((curr->ele._row == nNode->ele._row) && (curr->ele._column == nNode->ele._column)) {
        curr->ele._value = value;
        return;
      }
      curr = curr->next;
    }

    prev->next = nNode;
  }

  class const_iterator;

  /**
     Iteratore non costante della lista
   */
  
  class iterator {
  private:
    node *ptr; ///< nodo dove si trova l'iteratore
    friend class Matrice2D;

    /**
       Costruttore privato della classe iterator
       @param n nodo da assegnare all iteratore
     */
    explicit iterator(node *n) : ptr(n) {}

  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef element value_type;
    typedef ptrdiff_t difference_type;
    typedef element *pointer;
    typedef element &reference;

    /**
       Costruttore di default
     */
    iterator() : ptr(nullptr) {}

    /**
       Copy constructor
       @param other iteratore da copiare
     */
    iterator(const iterator &other) : ptr(other.ptr) {}
    /**
       Operatore di assegnamento
       @param other iteratore da cui vengono presi i valori da assegnare
     */
    iterator &operator=(const iterator &other) {
      ptr = other.ptr;
      return *this;
    }
    
    /**
       Distruttore della classe iterator
    */
    ~iterator() { ptr = nullptr; }

    /**
       Operatore di dereferenziamento
       @return elemento al nodo 
     */
    reference operator*() const { return ptr->ele; }

    /**
       Operatore ->
       @return puntatore all elemento al nodo 
     */
    pointer operator->() const { return &(ptr->ele); }
    

    /**
       Operatore di incremento postfisso

       @return l'iteratore non avanzato di una posizione
     */
    iterator &operator++(int) {
      iterator tmp(*this);
      ptr = ptr->next;
      return tmp;
    }

    /**
       Operatore di incremento prefisso

       @return l'iteratore avanzato di una posizione
     */

    iterator &operator++() {
      ptr = ptr->next;
      return *this;
    }

    /**
       Operatore di uguaglianza tra iterator
       @param other iterator con cui confrontarsi

       @return true se gli iteratori sono uguali, false altrimenti
       
     */

    bool operator==(const iterator &other) const { return (ptr == other.ptr); }

    /**
       Operatore di disuguaglianza tra iterator
       @param other iterator con cui confrontarsi

       @return false se gli iteratori sono uguali, true altrimenti
       
     */
    
    bool operator!=(const iterator &other) const { return (ptr != other.ptr); }

    /**
       Operatore di uguaglianza tra iterator e  const iterator
       @param other const iterator con cui confrontarsi

       @return true se gli iteratori sono uguali, false altrimenti
       
     */
    
    bool operator==(const const_iterator &other) const { return (ptr == other.ptr); }

    /**
       Operatore di disuguaglianza tra iterator e const iterator
       @param other const iterator con cui confrontarsi

       @return false se gli iteratori sono uguali, true altrimenti
       
     */
    
    bool operator!=(const const_iterator &other) const { return (ptr != other.ptr); }
  };

  /**
     Ritorna il punto di partenza per l' iterator
     
     @return iteratore posizionato alla testa della lista che rappresenta la matrice
   */
  iterator begin() { return iterator(_head); }

  /**
     Ritorna il punto di arresto per l'iterator
     
     @return iteratore posizionato alla testa della lista che rappresenta la matrice
   */

  iterator end() { return iterator(nullptr); } // FINE CLASS ITERATOR

  /**
     Iteratore costante della lista
  */
  class const_iterator {
    const node *ptr;

  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef element value_type;
    typedef ptrdiff_t difference_type;
    typedef const element *pointer;
    typedef const element &reference;

    /**
      Costruttore di default
    */
    const_iterator() : ptr(nullptr) {}

    /**
      Costruttore di copia
      @param other iteratore da copiare
    */
    const_iterator(const const_iterator &other) : ptr(other.ptr) {}

    /**
      Operatore di assegnamento
      @param other iteratore da cui prendere i valori da assegnare
      @return iteratore con valori uguali a other
    */
    const_iterator &operator=(const const_iterator &other) {
      ptr = other.ptr;
      return *this;
    }
        
    /**
      Distruttore
    */
    ~const_iterator() {}

    /**
      Operatore di dereferenziamento
      @return elemento al nodo 
    */
    reference operator*() const { return ptr->ele; }
        
    /**
      Operatore di dereferenziamento
      @return puntatore all elemento al nodo 
    */
    reference operator->() const { return &(ptr->ele); }

    /**
      Operatore di incremento postfisso
      @return l'iteratore non avanzato di una posizione
    */
    const_iterator &operator++(int) {
      const_iterator tmp(*this);
      ptr = ptr->next;
      return tmp;
    }
    /**
      Operatore di incremento prefisso
      @return l'iteratore avanzato di una posizione
    */

    const_iterator &operator++() {
      ptr = ptr->next;
      return *this;
    }

    /**
      Operatore di uguaglianza tra const iterator
      @param other const iterator con cui confrontarsi
      @return true se gli iteratori sono uguali, false altrimenti
    */

    bool operator==(const const_iterator &other) const { return (ptr == other.ptr); }

    /**
      Operatore di disuguaglianza tra const iterator
      @param other const iterator con cui confrontarsi
      @return false se gli iteratori sono uguali, true altrimenti
    */  
    bool operator!=(const const_iterator &other) const { return (ptr != other.ptr); }

    friend class iterator;

    /**
      Operatore di confronto tra const iterator e iterator
      @param other iterator con cui confrontarsi
      @return true se gli iteratori sono uguali, false altrimenti  
    */
    bool operator==(const iterator &other) const { return (ptr == other.ptr); }

    /**
      Operatore di confronto tra const iterator e iterator
      @param other iterator con cui confrontarsi
      @return false se gli iteratori sono uguali, true altrimenti
    */  
    bool operator!=(const iterator &other) const { return (ptr != other.ptr); }

    /**
      Costruttore da un iterator
      @param other, iteratore da cui copiare
    */
    explicit const_iterator(const iterator &other) : ptr(other.ptr) {}

    /**
      Operatore di assegnamento tra const iterator
      @param other const iterator da copiare
      @return const_iterator con posizione uguale alla posizione di other
    */
    const_iterator &operator=(const iterator &other) {
      ptr = other.ptr;
      return *this;
    }

  private:
    friend class Matrice2D;
        
    /**
    Costruttore privato di const_iterator
    @param n, nodo da cui partire con l'iteratore
    */
    explicit const_iterator(const node *other) : ptr(other) {}
  };

  /**
    Ritorna il punto di partenza per il const iterator
    @return iteratore posizionato alla testa della lista che rappresenta la matrice
  */
  
  const_iterator begin() const { return const_iterator(_head); }

  /**
    Ritorna il punto di arresto per il const iterator
    @return iteratore con valore nullptr
   */
  
  const_iterator end() const { return const_iterator(nullptr); }

  public:
    const T &operator()(int x, int y) const {
      return 0;
    }
};// FINE CLASSE Matrice2D

template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrice2D<T> &m) {
  typename Matrice2D<T>::const_iterator i, ie;
    
  i = m.begin();
  ie = m.end();

  while (i != ie) {
    os << *i << "";
    ++i;
  }

  return os;
}

#endif