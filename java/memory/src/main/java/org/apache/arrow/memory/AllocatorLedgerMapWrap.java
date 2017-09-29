/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.arrow.memory;

import java.util.Collection;
import java.util.IdentityHashMap;
import java.util.Map;
import java.util.Set;

import com.google.common.base.Preconditions;

/**
 * Wrapper on top of IdentityHashMap that allows
 * to deal with small number of items without going into real HashMap
 * For the future - shrink map when size goes significantly down
 */
public class AllocatorLedgerMapWrap implements Map<BaseAllocator,AllocationManager.BufferLedger> {

  private static int MAX_LINEAR_ELEMENTS = 3;

  private Object [] table;
  private IdentityHashMap<BaseAllocator,AllocationManager.BufferLedger> internalMap;
  private int size;

  AllocatorLedgerMapWrap() {
    table = new Object[2];
  }

  @Override
  public int size() {
    return (internalMap != null) ? internalMap.size() : size;
  }

  @Override
  public boolean isEmpty() {
    return (internalMap != null) ? internalMap.isEmpty() : (size == 0) ? true : false;
  }

  @Override
  public boolean containsKey(Object key) {
    if (internalMap != null) {
      return internalMap.containsKey(key);
    }

    Preconditions.checkState(size <= MAX_LINEAR_ELEMENTS);
    final Object[] tab = table;
    final int tabLength = tab.length;
    for (int i = 0; i < tabLength; i++) {
      if (tab[i] != null) {
        AllocationManager.BufferLedger value = (AllocationManager.BufferLedger) tab[i];
        if (value.allocator == key) {
          return true;
        }
      }
    }
    return false;
  }

  @Override
  public boolean containsValue(Object value) {
    if (internalMap != null) {
      return internalMap.containsValue(value);
    }

    Preconditions.checkState(size <= MAX_LINEAR_ELEMENTS);
    final Object[] tab = table;
    final int tabLength = tab.length;
    for (int i = 0; i < tabLength; i++) {
      if (tab[i] != null) {
        AllocationManager.BufferLedger bufLedger = (AllocationManager.BufferLedger) tab[i];
        if (bufLedger == value) {
          return true;
        }
      }
    }
    return false;
  }

  @Override
  public AllocationManager.BufferLedger get(Object key) {
    if (internalMap != null) {
      return internalMap.get(key);
    }

    Preconditions.checkState(size <= MAX_LINEAR_ELEMENTS);
    final Object[] tab = table;
    final int tabLength = tab.length;
    for (int i = 0; i < tabLength; i++) {
      if (tab[i] != null) {
        AllocationManager.BufferLedger bufLedger = (AllocationManager.BufferLedger) tab[i];
        if (bufLedger.allocator == key) {
          return bufLedger;
        }
      }
    }
    return null;
  }

  @Override
  public AllocationManager.BufferLedger put(BaseAllocator key, AllocationManager.BufferLedger value) {
    if (internalMap != null) {
      AllocationManager.BufferLedger oldValue = internalMap.put(key, value);
      size = internalMap.size();
      return oldValue;
    }

    final int currentSize = size;
    if (currentSize < MAX_LINEAR_ELEMENTS) {
      int emptyIndex = -1;
      final Object[] tab = table;
      final int tabLength = tab.length;
      for (int i = 0; i < tabLength; i++) {
        if (tab[i] != null) {
          AllocationManager.BufferLedger bufLedger = (AllocationManager.BufferLedger) tab[i];
          if (bufLedger.allocator == key) {
            return bufLedger;
          }
        } else {
          emptyIndex = i;
        }
      }
      // insert at empty index if available
      if (emptyIndex == -1) {
        // no empty index - increase size
        Object[] newTable = new Object[currentSize + 1];
        for (int i = 0; i < tabLength; i++) {
          newTable[i] = tab[i];
          tab[i] = null;
        }
        table = newTable;
        emptyIndex = currentSize;
      }
      if (emptyIndex > -1) {
        table[emptyIndex] = value;
        size++;
        return null;
      }
    } else {
      // we crossed the boundary
      internalMap = new IdentityHashMap<>(MAX_LINEAR_ELEMENTS);
      for (int i = 0; i < table.length; i++) {
        if (table[i] != null) {
          AllocationManager.BufferLedger bufLedger = (AllocationManager.BufferLedger) table[i];
          internalMap.put(bufLedger.allocator, bufLedger);
          table[i] = null;
        }
      }
      table = null;
      AllocationManager.BufferLedger oldValue = internalMap.put(key, value);
      size = internalMap.size();
      return oldValue;
    }
    throw new IllegalStateException("Should not reach here");
  }

  @Override
  public AllocationManager.BufferLedger remove(Object key) {
    if (internalMap != null) {
      AllocationManager.BufferLedger oldValue = internalMap.remove(key);
      size = internalMap.size();
      return oldValue;
    }

    Preconditions.checkState(size <= MAX_LINEAR_ELEMENTS);
    final Object[] tab = table;
    final int tabLength = tab.length;
    for (int i = 0; i < tabLength; i++) {
      if (tab[i] != null) {
        AllocationManager.BufferLedger bufLedger = (AllocationManager.BufferLedger) tab[i];
        if (bufLedger.allocator == key) {
          tab[i] = null;
          size--;
          return bufLedger;
        }
      }
    }
    return null;
   }

  @Override
  public void putAll(Map<? extends BaseAllocator, ? extends AllocationManager.BufferLedger> m) {
    if (internalMap != null) {
      internalMap.putAll(m);
      return;
    }
    throw new UnsupportedOperationException("putAll() method is not supported for map size < " + MAX_LINEAR_ELEMENTS);
  }

  @Override
  public void clear() {
    if (internalMap != null) {
      internalMap.clear();
      internalMap = null;
    }
    Object[] tab = table;
    for (int i = 0; i < tab.length; i++) {
      tab[i] = null;
    }
    size = 0;
  }

  @Override
  public Set<BaseAllocator> keySet() {
    if (internalMap != null) {
      return internalMap.keySet();
    }
    throw new UnsupportedOperationException("keySet() method is not supported for map size < " + MAX_LINEAR_ELEMENTS);
  }

  public AllocationManager.BufferLedger getNextValue() {
    if (internalMap != null) {
      return internalMap.values().iterator().next();
    }
    Preconditions.checkState(size <= MAX_LINEAR_ELEMENTS);
    final Object[] tab = table;
    final int tabLength = tab.length;
    for (int i = 0; i < tabLength; i++) {
      if (tab[i] != null) {
        AllocationManager.BufferLedger bufLedger = (AllocationManager.BufferLedger) tab[i];
        return bufLedger;
      }
    }
    return null;
  }

  @Override
  public Collection<AllocationManager.BufferLedger> values() {
    if (internalMap != null) {
      return internalMap.values();
    }

    throw new UnsupportedOperationException("values() method is not supported for map size < " + MAX_LINEAR_ELEMENTS);
  }

  @Override
  public Set<Entry<BaseAllocator, AllocationManager.BufferLedger>> entrySet() {
    if (internalMap != null) {
      return internalMap.entrySet();
    }
    throw new UnsupportedOperationException("entrySet() method is not supported for map size < " + MAX_LINEAR_ELEMENTS);
  }
}
