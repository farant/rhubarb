# Group D: Logic & Symbolic Reasoning — Paper Extractions

Date: 2026-02-26

---

## Paper 1: Tensor Logic: The Language of AI

**Authors:** Pedro Domingos (University of Washington)
**ArXiv:** 2510.12269v3, October 2025
**Venue:** N/A (preprint)

### Core Technique

Tensor logic proposes a unified programming language for AI where the sole construct is the *tensor equation*. The key observation is that logical rules (Datalog) and Einstein summation (einsum) are fundamentally the same operation: a Datalog rule is an einsum over Boolean tensors with a step function applied elementwise. A relation is just a compact representation of a sparse Boolean tensor. Therefore, all of symbolic AI (logic programming) and all of neural AI (tensor algebra) can be expressed in a single formalism.

A tensor logic program is a set of tensor equations. The LHS is the tensor being computed; the RHS is a series of tensor joins followed by tensor projection, with an optional univariate nonlinearity. Inference uses forward chaining (treat program as linear code, execute until fixpoint) or backward chaining (treat each equation as a function, recurse). Learning is simplified because the derivative of a tensor equation with respect to any RHS tensor is just the product of all the other RHS tensors. The entire gradient of a tensor logic program is itself a tensor logic program.

### Relevant Ideas

- **Datalog rules = einsums over Boolean tensors with step function** (Sec 3.1, p.5): A rule like `Aunt(x,z) <- Sister(x,y), Parent(y,z)` becomes `A_xz = H(S_xy * P_yz)` where H is Heaviside step. This is THE direct bridge between syllogisms and matrix operations. A syllogism IS a tensor equation.

- **Relations as sparse Boolean tensors** (Sec 3.1, p.4-5): Any n-ary relation can be represented as a rank-n Boolean tensor. The efficiency gain of sparse representation increases exponentially with n. For a small corpus like Bible + Summa, the relation tensors would be very sparse and very compact.

- **Single-layer perceptron as tensor equation** (Sec 3.1, p.6): `Y = step(W[i] X[i])` — joining on i implements the dot product. The entire language has only one construct: the tensor equation. No keywords, no other constructs.

- **Multilayer perceptron in one equation** (Sec 4.1, p.7): `X[i,j] = sig(W[i,j,k] X[i-1,k])` where i ranges over layers. Different layer sizes handled by implicit zero-padding.

- **Transformer in ~12 tensor equations** (Table 2, p.11): Complete transformer implementation including attention, layer norm, MLP, positional encoding — all as tensor equations. This is a remarkably compact specification.

- **Text as Boolean matrix** (Sec 3.1, p.6): Reading a text file produces a Boolean matrix where `M[i,j] = 1` iff position i contains word j. So the Bible becomes a sparse Boolean matrix. This is the input representation.

- **Forward chaining = deductive closure** (Sec 3.2, p.7): Execute tensor equations as linear code until no new elements can be computed. The result is the fixpoint/deductive closure. For syllogisms, this means: load all syllogisms as tensor equations, run forward chaining, get all possible inferences. This IS syllogism compilation.

- **Backward chaining = function calls** (Sec 3.2, p.7): Each tensor equation is treated as a function. Query calls the equation, which calls sub-equations recursively. This is the inference engine for answering questions.

- **Learning = specify loss + tensors to learn** (Sec 3.3, p.8): All tensors not supplied as training data are learned by default. The gradient is trivially computed because derivative of a tensor equation w.r.t. any tensor on RHS is just the product of all other tensors on RHS.

- **Backpropagation through structure** (Sec 3.3, p.8): In tensor logic, different equations may apply to different examples (unlike standard NNs where same architecture processes all examples). Solution: update each equation's parameters once for each time it appears in the example's derivation.

- **Tucker decomposition as predicate invention** (Sec 3.3, p.8): If the program to learn is `A[i,j,k] = M[i,p] M'[j,q] M''[k,r] C[p,q,r]`, the learned factor matrices M, M', M'' and core tensor C form a Tucker decomposition of A. Thresholding into Booleans yields invented predicates. This is how you DISCOVER new logical relations from data.

- **Reasoning in embedding space** (Sec 5, p.13-14): Embed facts/relations into dense vectors using random unit vectors. A relation R(x,y) embeds as `EmbR[i,j] = R(x,y) Emb[x,i] Emb[y,j]`. Rules embed as products of antecedent embeddings. Reasoning in this space gives approximately correct results, with error decreasing as embedding dimension increases. This is like a Bloom filter for logic.

- **Embedding a rule** (Sec 5, p.14): If rule is `Cons(...) <- Ant_1(...), ..., Ant_n(...)`, its embedding is `EmbCons[...] = EmbAnt_1[...] ... EmbAnt_n[...]`. You can embed an entire knowledge base (facts + rules) and reason over it using tensor operations.

- **Temperature-controlled reasoning** (Sec 5, p.15): Applying sigmoid with temperature T to each equation: T->0 makes reasoning purely deductive (Gram matrix becomes identity); T->infinity makes reasoning analogical (similar objects borrow inferences). Different T values can apply to different rules — e.g., mathematical truths use T=0, weak-evidence rules use high T.

- **Transparency of tensor logic reasoning** (Sec 5, p.15): Inferred tensors can be extracted at any point during inference. Unlike LLMs, the reasoning chain is fully transparent and, at low temperature, immune to hallucination.

- **Scaling via Tucker decomposition** (Sec 6, p.15-16): Convert sparse tensors to dense via Tucker decomposition. This is exponentially more efficient than operating on sparse tensors directly. Even random decomposition works (with small controllable error). This means a small knowledge base can be compressed into very compact dense tensors.

- **RNN in tensor logic is Turing-complete** (Sec 4.1, p.7): `X[i,*t+1] = sig(W[i,j] X[j,*t] + V[i,j] U[j,t])`. The `*t` notation means virtual index (no memory allocated, overwritten in place). Since RNNs are Turing-complete (Siegelmann and Sontag, 1995), tensor logic is Turing-complete.

- **Datalog programs are valid tensor logic programs** (Sec 4.2, p.11): Anything expressible in Datalog can be directly expressed in tensor logic. This means all symbolic reasoning just works.

- **Kernel machines in tensor logic** (Sec 4.3, p.11): `Y[Q] = f(A[i] Y[i] K[Q,i] + B)`. Polynomial kernel: `K[i,i'] = (X[i,j] X[i',j])^n`. Gaussian kernel: `K[i,i'] = exp(-(X[i,j] - X[i',j])^2 / Var)`.

- **Graphical models in tensor logic** (Sec 4.4, Table 3, p.12): Factor = tensor. Marginalization = projection. Pointwise product = join. Belief propagation = forward chaining. Sampling = selective projection.

- **GNN in tensor logic** (Table 1, p.10): Full graph neural network specified in 7 equations. Message passing, aggregation, update — all tensor equations.

### Relevance to Eigenflower Goals

| Goal | Relevance | How |
|------|-----------|-----|
| Compile syllogisms to ANN weights | **DIRECT HIT** | A syllogism IS a tensor equation over Boolean tensors. "All men are mortal, Socrates is a man, therefore Socrates is mortal" becomes `Mortal[x] = H(Man[x,y] Mortal_class[y])` or similar. The syllogism literally compiles to a matrix multiplication + step function. |
| Extract syllogisms from text | **DIRECT HIT** | Tucker decomposition on data tensors discovers latent predicates (predicate invention). Text -> Boolean matrix -> learn tensor equations -> threshold to Boolean -> extract rules. The learned factor matrices ARE the extracted syllogisms. |
| Dynamic weight recompilation | **STRONG** | New syllogism = new tensor equation. Adding it to the program just means adding one more equation. Forward chaining recomputes the deductive closure. No need to retrain — just add the equation and re-run inference. |
| Train in 30-60 min on M2 | **STRONG** | Tensor operations are what M2's Neural Engine and GPU are optimized for. Sparse Boolean tensors for a small corpus would be tiny. Tucker decomposition can compress aggressively. |
| Small corpus (Bible + Summa) | **STRONG** | Relations from these texts would produce very sparse tensors. The embedding space approach means you can pack a lot of knowledge into moderate-dimensional dense vectors. |
| Performant inference | **STRONG** | Forward/backward chaining over tensor equations maps directly to matrix multiplications — exactly what hardware accelerators do. |
| Diffusion text generation | **MODERATE** | Not directly addressed, but the graphical model implementation (Sec 4.4) provides the probabilistic framework needed for diffusion models. |
| Decompile existing models | **MODERATE** | Tucker decomposition of a model's weight tensors could reveal latent logical structure. Temperature analysis could reveal which parts of a model are doing deductive vs. analogical reasoning. |

### Implementation Notes

- No code released. The paper is a language specification, not an implementation.
- The paper explicitly discusses implementing tensor logic in CUDA as a next step.
- Mentions tensor-logic.org for more information.
- For eigenflower's C89 approach: tensor equations map cleanly to nested loops with array indexing. A tensor logic interpreter in C89 would be: parse equations, allocate tensors, execute forward/backward chaining via matrix operations.
- Sparse tensor representation (as relation tuples) is memory-efficient for small corpora.
- Dense operations for the learned embeddings can use Accelerate framework on macOS.

### Combinability

- **With Paper 2 (Hilbert Space):** Tensor logic's embedding space reasoning is essentially working in a Hilbert space. The temperature-controlled sigmoid is a kernel-like operation. Paper 2's RKHS framework could provide the theoretical foundation for choosing embedding dimensions, kernels, and analyzing convergence guarantees for tensor logic reasoning.
- **With Paper 3 (Edge of Chaos):** The complexity of the training data matters. For eigenflower, the Bible and Summa Theologica are at the "edge of chaos" — structured enough to learn from but complex enough to develop rich representations. Tensor logic's ability to mix Boolean (simple) and real-valued (complex) reasoning maps onto the complexity spectrum.
- **Key synthesis:** Syllogisms as Boolean tensor equations (Paper 1) + spectral decomposition for efficient representation (Paper 2) + train on data at the right complexity level (Paper 3). The syllogism compiler writes Boolean tensor equations. The learning engine discovers new equations from text using Tucker decomposition. The spectral framework ensures compact representation. The complexity analysis guides data curation.

---

## Paper 2: Operator-Based Machine Intelligence: A Hilbert Space Framework for Spectral Learning and Symbolic Reasoning

**Authors:** Andrew Kiruluta, Andreas Lemos, Priscilla Burity (UC Berkeley, School of Information)
**ArXiv:** 2507.21189v1, July 2025
**Venue:** N/A (preprint/report)

### Core Technique

This paper reframes machine learning not as parameter optimization in a neural network but as *operator estimation* in infinite-dimensional Hilbert spaces. Data points are treated as functions in a Hilbert space H, and learning becomes identifying a bounded linear or compact nonlinear operator T : H_X -> H_Y that maps input functions to output functions. The framework unifies RKHS kernel methods, spectral transforms (Fourier, wavelet, scattering), Koopman operators, and symbolic reasoning under a single mathematical umbrella.

The key innovation for reasoning is Section 8: reasoning is formulated as the application of a sequence of functional transformations R_i : H -> H that simulate logical relations. Given entities embedded as functions f_A, f_B in H, a reasoning operator T satisfying Tf_A ~ f_B encodes "A implies B" or "A is related to B by R." Transitive inference becomes operator composition: T_r2 * T_r1 * f_A ~ f_C. This can be learned from training pairs via closed-form least-squares in the Hilbert-Schmidt norm, or via spectral modulation of basis coefficients.

### Relevant Ideas

- **Learning as operator estimation** (Sec 4, p.5): Instead of optimizing finite parameters, learn a mapping T between function spaces. The optimization problem is `min sum ||Tf_i - g_i||^2 + lambda ||T||_S^2` where ||T||_S is a regularization norm (e.g., Hilbert-Schmidt). This has CLOSED-FORM solutions in many cases, unlike neural network training which requires iterative gradient descent.

- **Representer Theorem** (Sec 5, p.6): The minimizer of a regularized risk in RKHS must lie in the span of kernel sections at training points: `f*(x) = sum_i alpha_i K(x, x_i)`. This reduces infinite-dimensional optimization to a finite-dimensional problem of size n (number of training points). For a small corpus, n is small, making this very tractable.

- **Kernel ridge regression — closed-form solution** (Sec 5, p.6): `alpha = (K + lambda*I)^{-1} y` where K is the kernel matrix. No iterative training needed. For n training points, this is an n x n matrix inversion — doable on a MacBook for reasonable n.

- **Scattering transforms require NO training** (Sec 6.2, p.9; Sec 7.1, p.10): Cascading wavelet transforms with modulus nonlinearities produces translation-invariant, deformation-stable features WITHOUT any learned parameters. Achieved 98.6% on CUReT texture classification with just a linear SVM on top. Competitive with deep CNNs on TIMIT speech recognition with far fewer parameters. This is a feature extraction method that costs zero training time.

- **Functional composition as reasoning** (Sec 8, p.13): Given f_A, f_B in H, define reasoning operator T such that Tf_A ~ f_B. Learn T via closed-form least squares: `T = argmin sum ||Tf_{A_i} - f_{B_i}||^2 + lambda ||T||_HS^2`. Multiple relations = family of operators {T_r}. Transitive inference = operator composition: T_r2 * T_r1 * f_A ~ f_C. This is syllogistic reasoning as operator composition.

- **Reasoning with kernelized representations** (Sec 8, p.13): For binary relation R on domain X, define kernel `K_R((x,y),(x',y')) = <Phi(x) tensor Phi(y), Phi(x') tensor Phi(y')>`. Reasoning over chains (x -> y -> z) = composition in tensor product space: `K_{R2 o R1}(x,z) = integral K_{R1}(x,y) K_{R2}(y,z) d mu(y)`. This mirrors tensor-product kernels in knowledge base completion.

- **Spectral reasoning** (Sec 8, p.13): Represent entity as spectral expansion f_x = sum f_k^(x) * phi_k. A reasoning operator R acts as sparse modulation of frequency components: `R[f_x] = sum gamma_k^(r) * f_k^(x) * phi_k`. Word analogy "king - man + woman = queen" becomes spectral shift. This means syllogistic operations can be implemented as element-wise multiplication of spectral coefficients.

- **Learning to reason in Hilbert space** (Sec 8, p.14): Learn parameterized family of operators {T_theta^(r)} by minimizing `sum ||T_theta^(R_i) f_{x_i} - f_{y_i}||^2 + lambda R(theta)`. The operators can be linear maps, convolutional filters in basis domain, or neural networks mapping spectral coefficients.

- **Spectral Dictionary VLM (SDict-VLM)** (Sec 7.3, p.11): Replace self-attention with spectral decomposition: `x = sum alpha_i phi_i` where phi_i are learnable basis functions (Gabor, cosine, wavelet atoms) and alpha_i are sparse codes. Achieves 85% of BLIP-2 performance with 60% fewer parameters and 2.3x less peak memory. Eliminates quadratic attention complexity.

- **Koopman operators for dynamical systems** (Sec 7.2, p.11): Linear operators on function spaces that capture nonlinear dynamics. Trained via Extended Dynamic Mode Decomposition (EDMD). Enable long-term prediction, spectral interpretability, eigendecomposition for mode analysis. Outperform RNNs and LSTMs with fewer parameters.

- **Learnable spectral models** (Sec 6.3, p.10): Differentiable soft-thresholding in spectral domain: `f_k^out = sigma_{theta_k}(|f_k|) * f_k` where `sigma_{theta}(z) = z / (z + theta)`. This is a tunable band-pass filter that adapts per frequency component. Very few parameters per "layer."

- **Fourier spectral filtering** (Sec 6.1, p.8): Apply learnable multiplicative weights gamma_k to Fourier coefficients. This defines a diagonal operator in Fourier domain = convolution in spatial domain. Extremely efficient via FFT.

- **Compressed sensing** (Sec 6, p.7): If signal is sparse in some basis, it can be exactly recovered from far fewer measurements than Nyquist requires. Solve via L1 minimization: `min ||alpha||_1 subject to y = Phi*Psi*alpha`. This is relevant for compact representation of knowledge.

- **Orthogonal projection in Hilbert space** (Sec 3, p.4): Any function can be projected onto a closed subspace — the "shadow" is the closest point. This is the mathematical basis for all the approximation results. Learning = finding the best projection.

- **Parseval expansion** (Sec 6, p.7): Any function in H can be written as `f = sum <f, psi_n> psi_n` where {psi_n} is complete orthonormal basis. The norm is preserved: `||f||^2 = sum |<f, psi_n>|^2`. This means you can represent knowledge functions as coefficient vectors and operate on them efficiently.

- **Hilbert-Schmidt norm for operator regularization** (Sec 4, p.5): `||T||_HS^2 = sum ||T phi_i||^2` where {phi_i} is orthonormal basis of H_X. The HS class forms a separable Hilbert space itself. This gives you a principled way to regularize reasoning operators.

- **Data-driven basis adaptation** (Sec 9, p.16): Fixed bases (Fourier, wavelet) may not align with data. Future direction: learn orthonormal or frame-based dictionaries adapted to data distribution. Approaches include sparse coding, dictionary learning in RKHS, graph Laplacians.

- **Comparative advantages** (Sec 8.1, p.14-15): Hilbert space models are compact (scattering: no training; Koopman: low-rank matrices; spectral dictionary: sparse in frequency domain). They offer Lipschitz continuity, stability under deformations, guaranteed energy conservation. Particularly attractive for low-data regimes, edge computing, resource-constrained settings.

### Relevance to Eigenflower Goals

| Goal | Relevance | How |
|------|-----------|-----|
| Compile syllogisms to ANN weights | **DIRECT HIT** | Syllogism "A implies B" compiles to reasoning operator T where Tf_A ~ f_B. Multiple syllogisms = family of operators. Transitive chains = operator composition. The operators CAN be represented as matrices (finite-dimensional approximation) which ARE neural network weight matrices. |
| Extract syllogisms from text | **STRONG** | Embed text entities in Hilbert space via scattering transform or kernel embedding. Learn reasoning operators from (premise, conclusion) pairs. The learned operators ARE the extracted syllogistic relationships. Spectral decomposition of operators reveals which "frequency components" of meaning are relevant to each relation. |
| Dynamic weight recompilation | **STRONG** | Adding a new syllogism = adding a new (premise, conclusion) pair to the training set and re-solving the closed-form kernel regression. No iterative retraining needed — it's a matrix equation. For rank-1 updates (single new data point), Sherman-Morrison formula gives O(n^2) update instead of O(n^3) full re-solve. |
| Train in 30-60 min on M2 | **DIRECT HIT** | Scattering transforms need NO training. Kernel methods have CLOSED-FORM solutions. Spectral methods use FFT (O(n log n)). For a small corpus, the kernel matrix is small and invertible quickly. This could train in SECONDS, not minutes. |
| Small corpus (Bible + Summa) | **DIRECT HIT** | RKHS methods are specifically highlighted as ideal for "low-data regimes" (Sec 8.1). The Representer Theorem means the model complexity adapts to the data size. Small corpus = small kernel matrix = fast exact solution. |
| Performant inference | **STRONG** | Inference is inner products and matrix-vector multiplications. Spectral operations use FFT. Scattering is a fixed cascade of convolutions. All highly optimized on Apple silicon. |
| Diffusion text generation | **MODERATE** | Paper mentions combining Hilbert space models with generative modeling (Sec 9, p.16-17): "using spectral priors in VAEs or score-based models." Score-based = diffusion. Spectral priors could regularize the diffusion process. |
| Decompile existing models | **STRONG** | Eigendecomposition of weight matrices reveals the spectral structure. Koopman-style analysis of a model's dynamics could reveal what operators it has learned. The operator perspective makes this natural. |

### Implementation Notes

- No code released with this specific paper.
- References SDict-VLM (arXiv:2506.18943) by same authors for spectral dictionary implementation.
- Scattering transform implementations exist in multiple languages (Kymatio for Python; would need C89 port).
- Key operations for C89 implementation: FFT (well-known algorithm), matrix inversion (LU/Cholesky), kernel evaluation (inner products), wavelet transform (filter bank).
- For M2 MacBook: Accelerate framework provides BLAS/LAPACK/vDSP which handle all the core operations (matrix multiply, FFT, eigendecomposition).
- The 67k-parameter "Tiny" model from Paper 3 suggests even very small models can be effective — the Hilbert space approach would likely need even fewer parameters.

### Combinability

- **With Paper 1 (Tensor Logic):** Tensor logic provides the language for expressing reasoning; the Hilbert space framework provides the mathematical foundations for making that reasoning approximate but efficient. Tensor logic's embedding space reasoning (Sec 5) IS a finite-dimensional approximation to reasoning in a Hilbert space. The temperature parameter in tensor logic corresponds to kernel bandwidth in RKHS.
- **With Paper 3 (Edge of Chaos):** The scattering transform (which requires no training) is a fixed feature extractor that maps data to an invariant representation. The complexity of these representations could be analyzed using Paper 3's complexity measures. The "edge of chaos" insight suggests choosing scattering transform parameters that produce representations at intermediate complexity.
- **Key synthesis for syllogism compilation:** Represent terms (entities from Bible/Summa) as functions in RKHS via kernel embedding. Each syllogism form (Barbara, Celarent, Darii, Ferio, etc.) becomes a reasoning operator T. The operator is learned in closed form from examples. New syllogisms produce new operators via Sherman-Morrison update. Inference chains = operator composition. Spectral decomposition reveals which aspects of meaning each syllogism operates on.

---

## Paper 3: Intelligence at the Edge of Chaos

**Authors:** Shiyang Zhang, Aakash Patel, Syed Rizvi, Nianchen Liu, Sizhuang He, Amin Karbasi, Emanuele Zappala, David van Dijk (Yale, Columbia, Northwestern, Idaho State)
**ArXiv:** 2410.02536v3, March 2025
**Venue:** ICLR 2025

### Core Technique

This paper trains separate GPT-2 instances on data generated by individual Elementary Cellular Automata (ECA) rules, then evaluates the models' learned representations on downstream tasks (reasoning, chess, Nim). The key finding is that models trained on data at intermediate complexity — the "edge of chaos" (Wolfram Class IV rules like Rule 110) — develop the best general-purpose representations, outperforming models trained on both simple (Class I/II) and maximally chaotic (Class III) data.

The methodology: (1) Generate binary spatiotemporal data from each of 88 unique ECA rules, (2) Train modified GPT-2 models to predict next states, (3) Freeze transformer layers and train only input/output projections on downstream tasks, (4) Measure correlation between data complexity (Lempel-Ziv, compression, Lyapunov, Krylov) and downstream performance. The correlation is strongly positive (r=0.73 for easy reasoning, r=0.44 for chess) with a clear peak at Class IV.

### Relevant Ideas

- **Intelligence emerges from predicting complexity, not from intelligent data** (Sec 1, p.1; Sec 7, p.9): The training data is generated by a deterministic 8-bit rule with no inherent intelligence. Yet the learned representations transfer to reasoning and strategic tasks. This means you do NOT need "intelligent" training data to produce intelligent behavior. The structure of the data matters more than its source.

- **Edge of chaos = optimal complexity for learning** (Sec 7, p.9): Class IV ECA rules (like Rule 110, which is Turing-complete) produce structured-yet-unpredictable data. Models trained on this data develop the richest representations. Too simple (Class I/II) = trivial solutions. Too chaotic (Class III) = noise, no learnable patterns. The sweet spot is intermediate complexity.

- **Models learn complex solutions even when simple ones exist** (Sec 5.2, p.6-7): ECA is memoryless — the next state depends only on the current state. The trivial solution is to learn the 8-bit rule and apply it to the current state. But models trained on complex ECA data attend to historical states (r=0.66 correlation between complexity and attention to past states). They learn MORE than they need to, and this surplus complexity is what makes them useful for downstream tasks.

- **Data complexity > data quantity** (Sec 7, p.9; Appendix A, p.14): Complex data requires more tokens to converge but produces better representations. This aligns with Hoffmann et al. (2022): data quality/complexity matters more than just quantity. For eigenflower's small corpus, this means the COMPLEXITY of the Bible and Summa (which is very high — multilingual, philosophical, theological) matters more than its size.

- **Temporal structure is critical** (Appendix B, p.14-15): Randomly shuffling the temporal order of ECA states significantly degrades downstream performance. Both spatial complexity AND temporal structure are needed. For text, this means preserving the sequential/argumentative structure matters — don't just bag-of-words it.

- **Modified GPT-2 for binary data** (Sec 3.2, p.4): Replaced token embeddings with a linear projection layer that maps binary vectors directly into embedding space. Output is also a linear projection back to binary. No vocabulary, no softmax over tokens. This is a much simpler architecture that could be adapted for eigenflower.

- **Tiny model (67k params) still shows the effect** (Appendix A, p.14): A single-layer, single-head transformer with 64-dimensional embeddings and only 67,000 parameters shows the same complexity-intelligence correlation (r=0.71). This proves the phenomenon is not about model scale — even tiny models benefit from complex training data.

- **Scaling: larger models learn complex data more efficiently** (Appendix A, p.14): The 85M "Small" model converges faster than the 67k "Tiny" model on complex data. But the 67k model still works. This means eigenflower can start very small and scale up as needed.

- **Complexity measures as data curation tools** (Sec 2.3, p.2-3; Sec 7, p.9-10): Lempel-Ziv complexity, compression complexity, Lyapunov exponent, Krylov complexity all correlate with downstream performance. These could be used to SCORE and CURATE the training corpus — select passages from the Bible/Summa that are at the right complexity level.

- **Wolfram Class IV rules are Turing-complete** (Sec 2.1, p.2): Rule 110 is proven Turing-complete. The data generated by these rules contains ALL the computational structure needed for general intelligence. This is a deep point: universal computation lives at the edge of chaos.

- **Attention patterns reveal learning strategy** (Sec 5.2, Fig 3, p.7): Models on uniform data (Rule 168) ignore history. Models on periodic data (Rule 179) attend to periodic cycles. Models on complex data (Rule 110) attend broadly to history. The attention pattern reveals WHAT the model has learned. This could be used for interpretability.

- **Reservoir computing connection** (Sec 6, p.9): Reservoir computing uses fixed, randomly-initialized RNNs with only output weights trained. It projects inputs into high-dimensional space via inherent dynamics. The ECA-pretrained transformers are similar — the internal representations are fixed, only I/O layers adapt. This suggests a reservoir-computing-style architecture for eigenflower.

- **Non-trivial solutions outperform trivial ones even for simple tasks** (Sec 5.2, p.7; Sec 7, p.9): Models that learn complex internal representations (attending to history) perform better on downstream tasks than models that learn the trivial solution (current-state-only). This suggests that "overlearning" the training data's structure is beneficial.

- **1-step prediction is sufficient** (Sec 5.2, p.7): They initially trained 5-step prediction expecting 1-step would be too easy. But even 1-step prediction on complex data produces rich representations. The task difficulty doesn't need to be high — the DATA complexity is what matters.

- **Pretraining setup** (Sec 3.3, p.4): Adam optimizer, lr = 2e-6, weight decay 0.01, linear warmup (10% of steps), cosine annealing, gradient clipping (max norm 1.0), batch size 64, sequences of 60 time steps x 100 spatial dimensions. Early stopping on validation loss. Up to 10,000 epochs.

- **Hardware** (Sec 4.3, p.5): 12 NVIDIA H100 GPUs (80GB each). But the Tiny model (67k params) would run trivially on a MacBook. The hardware was for running all 88 rules in parallel, not because individual models need it.

- **Irreducible systems develop approximate solutions** (Sec 7, p.10): Even for computationally irreducible systems (Rule 110), models learn approximate-but-efficient prediction strategies. These approximate solutions generalize better than exact solutions. This is relevant to eigenflower: approximate syllogistic reasoning may be more useful than exact reasoning.

- **Chess move prediction as reasoning benchmark** (Sec 4.2, p.5): ECA-pretrained models transfer to chess (r=0.44 correlation). Chess requires multi-step strategic reasoning. This suggests that simple-data pretraining can bootstrap complex reasoning abilities.

- **Nim game benchmark** (Appendix C, p.15): Nim requires binary addition (XOR of heap sizes). ECA-pretrained models show r=0.63 correlation between pretraining complexity and Nim performance. Nim's logical structure is closer to syllogistic reasoning than chess.

### Relevance to Eigenflower Goals

| Goal | Relevance | How |
|------|-----------|-----|
| Compile syllogisms to ANN weights | **MODERATE** | The edge-of-chaos insight suggests that a model pretrained on sufficiently complex data will naturally develop internal representations amenable to logical reasoning, even if the pretraining data isn't explicitly logical. The syllogism compiler might benefit from a "warm start" with edge-of-chaos pretrained weights. |
| Extract syllogisms from text | **MODERATE** | The attention analysis technique (Sec 5.2) could reveal which parts of the input the model uses for inference — this could help identify which textual patterns correspond to syllogistic structures. |
| Dynamic weight recompilation | **LOW** | Not directly addressed. But the finding that only I/O layers need training (while internal representations are fixed) suggests that dynamic recompilation could focus on these outer layers only. |
| Train in 30-60 min on M2 | **DIRECT HIT** | The 67k-parameter Tiny model (1 layer, 1 head, 64-dim embeddings) shows the effect. Training such a model on a MacBook would take minutes, not hours. The paper validates that you do NOT need large models. |
| Small corpus (Bible + Summa) | **STRONG** | The key finding is that data COMPLEXITY matters more than quantity. The Bible and Summa are extremely complex texts (multilingual, philosophical, self-referential, historical). Even a small amount of highly complex data produces good representations. |
| Performant inference | **STRONG** | A 67k-parameter model runs trivially on any hardware. Even the 85M model would be fast on M2. |
| Diffusion text generation | **LOW** | Not directly addressed. |
| Decompile existing models | **MODERATE** | The complexity analysis framework (Lempel-Ziv, Lyapunov, Krylov) could be applied to the ACTIVATIONS of existing models to understand what complexity regime they operate in. The attention analysis technique could reveal reasoning strategies. |

### Implementation Notes

- **Code available:** https://github.com/vandijklab/Intelligence_at_the_edge_of_chaos
- PyTorch 2.1.2, Transformers library 4.41.0, CUDA 12.4.
- The GPT-2 modification (linear projection instead of token embedding) is simple and could be implemented in C89.
- The Tiny model (67k params) architecture: 1 layer, 1 attention head, 64-dim embeddings. This is easily implementable from scratch.
- ECA data generation is trivial in C89: each rule is an 8-bit lookup table, each step is a single pass over the array.
- Lempel-Ziv complexity is implementable as a simple string scanning algorithm.
- Key hyperparameters: lr=2e-6, weight_decay=0.01, batch_size=64, seq_len=60, spatial_dim=100.

### Combinability

- **With Paper 1 (Tensor Logic):** Use tensor logic to SPECIFY the syllogisms and reasoning rules. Use edge-of-chaos pretraining to give the model a "warm start" with rich internal representations. The tensor logic equations define the structure; the pretrained weights provide the substrate.
- **With Paper 2 (Hilbert Space):** The scattering transform (Paper 2) is a fixed feature extractor that maps data to invariant representations at multiple scales. The complexity of these representations could be analyzed using Paper 3's complexity measures. Use scattering as the feature extractor, then apply kernel methods for the reasoning layer. The scattering transform naturally operates at the edge of chaos — it captures structure at all scales while being stable to perturbations.
- **Key synthesis for eigenflower:** (1) Use complexity measures to curate training data from Bible/Summa — select passages at the edge of chaos. (2) Pretrain a tiny transformer (67k-85M params) on this curated data. (3) The pretrained model develops rich internal representations. (4) Then compile syllogisms as tensor equations (Paper 1) into the model's weight space, or learn reasoning operators (Paper 2) on top of the pretrained representations.

---

## Cross-Paper Synthesis: The Eigenflower Architecture

### The Syllogism Compilation Pipeline

Combining all three papers suggests a concrete architecture:

1. **Text Encoding (Paper 1 + Paper 2):** Bible/Summa text -> Boolean matrix (Paper 1) or kernel embedding (Paper 2). Each entity (term, concept, person) gets a vector representation either through learned embeddings or scattering transform features.

2. **Syllogism Compilation (Paper 1):** Each syllogism form maps to a tensor equation:
   - "All A are B" -> Boolean tensor `AllAB[a,b] = 1` for all (a,b) pairs
   - "Some A are B" -> partial Boolean tensor
   - Barbara: "All M are P, All S are M, therefore All S are P" -> `SP[s,p] = H(SM[s,m] * MP[m,p])` — one einsum + step function
   - This compiles DIRECTLY to a matrix multiplication weight matrix

3. **Reasoning Engine (Paper 1 + Paper 2):**
   - Forward chaining: execute all tensor equations until fixpoint = compute all derivable conclusions
   - Backward chaining: answer specific queries by recursing through equations
   - Operator composition (Paper 2): chain reasoning operators for multi-step inference
   - Temperature control (Paper 1): T=0 for deductive certainty, T>0 for analogical reasoning

4. **Dynamic Recompilation (Paper 1 + Paper 2):**
   - New syllogism -> new tensor equation -> add to program -> recompute deductive closure
   - Or: new (premise, conclusion) pair -> Sherman-Morrison update to kernel matrix -> O(n^2) update
   - No full retraining needed in either approach

5. **Pretraining Substrate (Paper 3):**
   - Optionally pretrain a tiny transformer on Bible/Summa text (or even ECA data) to develop rich internal representations
   - Then layer the syllogism compilation on top of these representations
   - The pretrained representations provide the "embedding space" that Paper 1's reasoning-in-embedding-space requires

### Model Size Estimates

- **67k params** (Paper 3 Tiny): 1 layer, 1 head, 64-dim — runs on anything, trains in minutes
- **85M params** (Paper 3 Small): 12 layers, 12 heads, 768-dim — runs on M2, trains in ~30-60 min
- **Kernel matrix** (Paper 2): For n training entities, need n x n matrix. Bible has ~3,000 unique names/entities, Summa has ~500 key concepts. So ~3,500 x 3,500 = ~12M entries. Trivially fits in memory.
- **Boolean tensors** (Paper 1): Sparse, stored as relation tuples. Bible relations might be ~100k tuples. Summa syllogisms might be ~10k. Tiny.

### Hardware Fit (M2 MacBook)

- Accelerate framework: BLAS for matrix multiply, LAPACK for matrix inversion/eigendecomposition, vDSP for FFT
- Neural Engine: 15.8 TOPS for tensor operations
- Metal GPU: for parallel tensor equation evaluation
- All three papers' core operations map to operations that are highly optimized on Apple silicon
- The closed-form solutions (Paper 2) and Boolean tensor operations (Paper 1) avoid the need for GPU-intensive iterative training

### What to Build First

Based on the three papers, the minimal viable system would be:

1. A tensor equation interpreter in C89 (from Paper 1): parse `LHS = f(RHS1 * RHS2 * ... )`, execute forward/backward chaining
2. A Boolean relation store (from Paper 1): store facts as sparse tuples, convert to tensors for computation
3. A kernel matrix solver (from Paper 2): given training pairs, compute `alpha = (K + lambda*I)^{-1} y` for reasoning operators
4. A complexity scorer (from Paper 3): Lempel-Ziv complexity to evaluate/curate training data

This gives you: (a) syllogism compilation via tensor equations, (b) syllogism extraction via kernel learning, (c) dynamic recompilation via matrix update, (d) data curation via complexity scoring.
