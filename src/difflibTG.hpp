#include <string>
#include <list>
#include <functional>
#include <unordered_map>
#include <tuple>
#include <set>
#include <iostream>
#include <array>
#include <vector>

inline double _calculate_ratio(size_t length_matches, size_t length){
    if(length!=0)
        return 2.0 * length_matches / length;
    return 1.0;
}

//special implementation only for strings and nothing else!
//shoul be equally fast as the python method
class SequenceMatcher{

    public:
        typedef std::array<size_t, 3> match; //typedef match as a fixed size array
        
        SequenceMatcher() = delete;
        SequenceMatcher(const std::string& a,const std::string& b):a_(a),b_(b),b2j_(),matching_blocks_(), isjunk_(nullptr),autojunk_(true){
            chain_b_(); 
            junk_.clear();
            popular_.clear();
        }
    
        /*
        void set_seqs(const std::string* a, const std::string* b){
            set_seq1(a);
            set_seq2(b);
        } 
    
        void set_seq1(const std::string* a){
            if(a==a_) return;
            a_ = a;
            matching_blocks_.clear();
       }
    
        void set_seq2(const std::string* b){
            if(b==b_) return;
            b_ = b;
            matching_blocks_.clear();

            chain_b_();
        }
        */
        double real_quick_ratio() const{
            /*Return an upper bound on ratio() very quickly.
            
            This isn't defined beyond that it is an upper bound on .ratio(), and
            is faster to compute than either .ratio() or .quick_ratio().
            */

            auto la = a_.size();
            auto lb = b_.size(); 
            // can't have more matches than the number of elements in the
            // shorter sequence
            return _calculate_ratio(std::min(la, lb), la + lb);
        }
        
        double ratio(){
            /*Return a measure of the sequences' similarity (float in [0,1]).

            Where T is the total number of elements in both sequences, and
            M is the number of matches, this is 2.0*M / T.
            Note that this is 1 if the sequences are identical, and 0 if
            they have nothing in common.

            .ratio() is expensive to compute if you haven't already computed
            .get_matching_blocks(), in which case you may
            want to try .quick_ratio() or .real_quick_ratio() first to get an
            upper bound.
            */
            auto blocks = get_matching_blocks_();

            size_t matches = 0;
            for(const auto& elem: blocks)
                matches += elem[2];
            return _calculate_ratio(matches, a_.size() + b_.size());
        }
        
        
    private:      

        const std::string& a_; //sequence a
        const std::string& b_; //sequence b
        std::unordered_map<char, std::list<size_t>> b2j_;  
        //b2j[x] is a list of the indices at which x appears; junk elements do not appear
        std::list<match> matching_blocks_;
        //      a list of (i, j, k) triples, where a[i:i+k] == b[j:j+k];
        //      ascending & non-overlapping in i and in j; terminated by
        //     a dummy (len(a), len(b), 0) sentinel
        std::function<bool(char c)> *isjunk_;
        //      a user-supplied function taking a sequence element and
        //     returning true iff the element is "junk" -- this has
        //      subtle but helpful effects on the algorithm, which I'll
        //      get around to writing up someday <0.9 wink>.
        //     DON'T USE!  Only __chain_b uses this.  Use isbjunk.
        //std::function<bool(char c)> isbjunk_;
        //      for x in b, isbjunk(x) == isjunk(x) but much faster;
        //     it's really the __contains__ method of a hidden dict.
        //     DOES NOT WORK for x in a!
        //std::function<bool(char c)> isbpopular_;
        //      for x in b, isbpopular(x) is true iff b is reasonably long
        //      (at least 200 elements) and x accounts for more than 1 + 1% of
        //      its elements (when autojunk is enabled).
        //      DOES NOT WORK for x in a!
        
        //used for some internal stuff
        std::set<char> junk_;
        std::set<char> popular_;
        bool autojunk_;   
        static constexpr int AUTOJUNK_SIZE = 200;
        
        void chain_b_(){
        /*
        Because isjunk is a user-defined (not C) function, and we test
         for junk a LOT, it's important to minimize the number of calls.
         Before the tricks described here, __chain_b was by far the most
         time-consuming routine in the whole module!  If anyone sees
         Jim Roskind, thank him again for profile.py -- I never would
         have guessed that.
         The first trick is to build b2j ignoring the possibility
         of junk.  I.e., we don't call isjunk at all yet.  Throwing
         out the junk later is much cheaper than building b2j "right"
         from the start.
        */
        b2j_.clear();

        //add all elements in b to b2j_
        for(size_t i =0; i < b_.size(); ++i){
            //fill if not already present
            char c = b_[i];
            if(b2j_.find(c)==b2j_.end())
                b2j_[c] = std::list<size_t>{i};
            else
                b2j_[c].push_back(i);         
        }
                
        
        // Purge junk elements
        junk_.clear();
        if (isjunk_ !=nullptr){
            std::unordered_map<char, std::list<size_t>> b2j_copy = b2j_;
            for (const auto& x :b2j_copy){ //copy b2j_, since it will be purged!
                if( (*isjunk_)(x.first) ){
                    junk_.insert(x.first);
                    b2j_.erase(x.first);
                }
            }
        }
        
        // Purge popular elements that are not junk
        popular_.clear();
        if(autojunk_ and b_.size() >= AUTOJUNK_SIZE){
            size_t ntest = b_.size()/100 + 1;
            std::unordered_map<char, std::list<size_t>> b2j_copy = b2j_;
            for(const auto& elem :b2j_copy){
                if(elem.second.size() > ntest){
                    popular_.insert(elem.first);
                    b2j_.erase(elem.first);
                }    
            }
         }
        
    }
    
    
    inline bool isbpopular_(char c) const {return popular_.find(c)!=popular_.end();}
    inline bool isbjunk_(char c) const {return junk_.find(c)!=junk_.end();}


    std::list<match>& get_matching_blocks_(){
        /*Return list of triples describing matching subsequences.

        Each triple is of the form (i, j, n), and means that
        a[i:i+n] == b[j:j+n].  The triples are monotonically increasing in
        i and in j.  New in Python 2.5, it's also guaranteed that if
        (i, j, n) and (i', j', n') are adjacent triples in the list, and
        the second is not the last triple in the list, then i+n != i' or
        j+n != j'.  IOW, adjacent triples never describe adjacent equal
        blocks.

        The last triple is a dummy, (len(a), len(b), 0), and is the only
        triple with n==0.

        >>> s = SequenceMatcher(None, "abxcd", "abcd")
        >>> s.get_matching_blocks()
        [Match(a=0, b=0, size=2), Match(a=3, b=2, size=2), Match(a=5, b=4, size=0)]
        */
        if (matching_blocks_.size()!=0)
            return matching_blocks_;
            
        size_t la = a_.size();
        size_t lb = b_.size();

        // This is most naturally expressed as a recursive algorithm, but
        // at least one user bumped into extreme use cases that exceeded
        // the recursion limit on their box.  So, now we maintain a list
        // ('queue`) of blocks we still need to look at, and append partial
        // results to `matching_blocks` in a loop; the matches are sorted
        // at the end.
        
        
        std::list<std::tuple<size_t,size_t,size_t,size_t>> queue;
        queue.push_back(std::make_tuple(0,la,0,lb));
   
        while(not queue.empty() ){
            size_t alo, ahi, blo, bhi;
            std::tie(alo,ahi,blo,bhi) = queue.front();
            queue.pop_front();

            match m = find_longest_match_(alo, ahi, blo, bhi);
            //match is (i,j,k)
            // a[alo:i] vs b[blo:j] unknown
            // a[i:i+k] same as b[j:j+k]
            // a[i+k:ahi] vs b[j+k:bhi] unknown
            if(m[2] > 0){   // if k is 0, there was no matching block
                matching_blocks_.push_back(m);
                if(alo < m[0] and blo < m[1])
                    queue.push_back(std::make_tuple(alo, m[0], blo, m[1]));
                if( m[0]+m[2] < ahi and m[1]+m[2] < bhi)
                    queue.push_back(std::make_tuple(m[0]+m[2], ahi, m[1]+m[2], bhi));
            }
        }       
        
        //auto first_element = [] (const match& a,const match &b) { 
        //        return (match[0] <= match[1]); };
        matching_blocks_.sort();
              
        // It's possible that we have adjacent equal blocks in the
        // matching_blocks list now.  Starting with 2.5, this code was added
        // to collapse them.
        size_t i1 = 0, j1 = 0, k1 = 0;
        std::list<match> non_adjacent;
        for(const match& m: matching_blocks_){
            size_t i2 = m[0], j2 = m[1], k2 = m[2];
            
            // Is this block adjacent to i1, j1, k1?
            if( (i1 + k1 == i2) and (j1 + k1 == j2) ){
                // Yes, so collapse them -- this just increases the length of
                // the first block by the length of the second, and the first
                // block so lengthened remains the block to compare against.
                k1 += k2;
            }else{
                // Not adjacent.  Remember the first block (k1==0 means it's
                // the dummy we started with), and make the second block the
                // new block to compare against.
                if (k1!=0)
                    non_adjacent.push_back({{i1, j1, k1}});
                i1 = i2;
                j1 = j2;
                k1 = k2;
            }
        }        
        if (k1!=0)
            non_adjacent.push_back({{i1, j1, k1}});

        non_adjacent.push_back({{la, lb, 0}});
        matching_blocks_ = non_adjacent;
        
        return matching_blocks_;
    }
       
    //def find_longest_match(self, alo, ahi, blo, bhi):
    inline match find_longest_match_(size_t alo, size_t ahi, 
                                     size_t blo, size_t bhi) 
    {
        /* """Find longest matching block in a[alo:ahi] and b[blo:bhi].

        If isjunk is not defined:

        Return (i,j,k) such that a[i:i+k] is equal to b[j:j+k], where
            alo <= i <= i+k <= ahi
            blo <= j <= j+k <= bhi
        and for all (i',j',k') meeting those conditions,
            k >= k'
            i <= i'
            and if i == i', j <= j'

        In other words, of all maximal matching blocks, return one that
        starts earliest in a, and of all those maximal matching blocks that
        start earliest in a, return the one that starts earliest in b.

        >>> s = SequenceMatcher(None, " abcd", "abcd abcd")
        >>> s.find_longest_match(0, 5, 0, 9)
        Match(a=0, b=4, size=5)

        If isjunk is defined, first the longest matching block is
        determined as above, but with the additional restriction that no
        junk element appears in the block.  Then that block is extended as
        far as possible by matching (only) junk elements on both sides.  So
        the resulting block never matches on junk except as identical junk
        happens to be adjacent to an "interesting" match.

        Here's the same example as before, but considering blanks to be
        junk.  That prevents " abcd" from matching the " abcd" at the tail
        end of the second sequence directly.  Instead only the "abcd" can
        match, and matches the leftmost "abcd" in the second sequence:

        >>> s = SequenceMatcher(lambda x: x==" ", " abcd", "abcd abcd")
        >>> s.find_longest_match(0, 5, 0, 9)
        Match(a=1, b=0, size=4)

        If no blocks match, return (alo, blo, 0).

        >>> s = SequenceMatcher(None, "ab", "c")
        >>> s.find_longest_match(0, 2, 0, 1)
        Match(a=0, b=0, size=0)
        """

        # CAUTION:  stripping common prefix or suffix would be incorrect.
        # E.g.,
        #    ab
        #    acab
        # Longest matching block is "ab", but if common prefix is
        # stripped, it's "a" (tied with "b").  UNIX(tm) diff does so
        # strip, so ends up claiming that ab is changed to acab by
        # inserting "ca" in the middle.  That's minimal but unintuitive:
        # "it's obvious" that someone inserted "ac" at the front.
        # Windiff ends up at the same place as diff, but by pairing up
        # the unique 'b's and then matching the first two 'a's.
        */
        
        //a, b, b2j, isbjunk = self.a, self.b, self.b2j, self.isbjunk
        size_t besti    = alo;
        size_t bestj    = blo;
        size_t bestsize = 0;
        // find longest junk-free match
        // during an iteration of the loop, j2len[j] = length of longest
        // junk-free match ending with a[i-1] and b[j]
        std::unordered_map<size_t,size_t> j2len;
        for(size_t i = alo; i < ahi; ++i){
            // look at all instances of a[i] in b; note that because
            // b2j has no junk keys, the loop is skipped if a[i] is junk
            //j2lenget = j2len.get -> get element func 
            std::unordered_map<size_t,size_t> newj2len;
            if( b2j_.find(a_[i]) == b2j_.end() )
                continue;
            for(size_t j: b2j_[a_[i]]){
                // a[i] matches b[j]
                if (j < blo) //not in part to test
                    continue;
                if (j >= bhi)
                    break;
                const auto got = j2len.find(j-1);    
                size_t k = (got==j2len.end())?1:(got->second +1);//1 if not found
                newj2len[j] = k;
                if(k > bestsize){
                    besti    = i-k+1;
                    bestj    = j-k+1;
                    bestsize = k;
                }
            }

            j2len = std::move(newj2len); //faster copy of map, invalidated new2len
        }
        
        // Extend the best by non-junk elements on each end.  In particular,
        // "popular" non-junk elements aren't in b2j, which greatly speeds
        // the inner loop above, but also means "the best" match so far
        // doesn't contain any junk *or* popular non-junk elements.
        
        while ((besti > alo) and (bestj > blo) and 
              (not isbjunk_(b_[bestj-1])) and 
              (a_[besti-1] == b_[bestj-1]) ){
                besti -= 1;
                bestj -= 1;
                bestsize += 1;
            }
        while ((besti+bestsize < ahi) and (bestj+bestsize < bhi) and 
              (not isbjunk_(b_[bestj+bestsize])) and 
              (a_[besti+bestsize] == b_[bestj+bestsize]) )
            bestsize += 1;

        // Now that we have a wholly interesting match (albeit possibly
        // empty!), we may as well suck up the matching junk on each
        // side of it too.  Can't think of a good reason not to, and it
        // saves post-processing the (possibly considerable) expense of
        // figuring out what to do with it.  In the case of an empty
        // interesting match, this is clearly the right thing to do,
        // because no other kind of match is possible in the regions.
        while (  isbjunk_(b_[bestj-1]) and 
              (a_[besti-1] == b_[bestj-1])) {
            besti -= 1;
            bestj -= 1;
            bestsize +=1;
        }
        while ((besti+bestsize < ahi) and (bestj+bestsize < bhi) and 
              isbjunk_(b_[bestj+bestsize]) and 
              (a_[besti+bestsize] == b_[bestj+bestsize]) )
            bestsize += 1;

        return {besti, bestj, bestsize};
    
    }
    
    //def quick_ratio(self):
    
};        
