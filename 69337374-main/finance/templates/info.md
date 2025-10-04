# Information

[TOC]

## Price Difference Percentage

*"C$50 Finance"* uses the "average price" of a symbol over previous transactions to compare to the new and updated price, in order to calculate the price difference percentage (`% Change`).

$$
    \text{\% Change} = \frac{\text{Current Price} - \text{Average Price}}{\text{Average Price}} \cdot 100
$$

The specifics of calculating the average price are highlighted [here](#average-price).

## Average Price
The average price over previous transactions is calculated as the ratio of the sum of the totals to the sum of the shares.

- "Shares" and "Total" are (+) *positive* for *buying* shares.
- "Shares" and "Total" are (-) *negative* for *selling* shares.

**Example transactions (for one stock):**

| Shares | Price | TOTAL |
| --- | --- | -- |
| $+10$ | $\$15$ | $+\$150$ |
| $-5$  | $\$12$ | $-\$60$  |
| $-3$  | $\$10$ | $-\$30$  |

The **average** value of the symbol over previous transactions will be: $\frac{\$150-\$60-\$30}{10-5-3} = \$45$

Thus, if the symbol now costs more than $\$45$ per share, then it is profitable to sell.

### General form

$$ \overline{P}_n = \sum_{i=1}^{n} \frac{P_i S_i}{S_i} = \sum_{i=1}^{n} \frac{T_i}{S_i} $$

where $P_i$, $T_i$, $S_i$ are the price, total and shares respectively, bought in the $i$-th transaction.

$P_i$ is unsigned (always positive), wheread $T_i$ and $S_i \ne 0$ are signed (either positive or negative).

## Interpreting Average Price

> The **average price** of a symbol represents a value such that if the symbol costed that **price** right now, one would make exactly **zero net profit/loss** from selling all of its shares at that moment.

*Assuming 2 transactions ($i = \{1, 2\}$) and portion of shares being sold in the second transaction ($S_2 < S_1$) for simplicity.*

When counting sold (negative) shares, the following are true:

1. When the (some of the) shares are sold at a *lower* price than their original, the average price *increases*.
    $$ P_2 < P_1 \implies \overline{P}_2 > \overline{P}_1 $$

    **Reason:** Some loss has been incurred. Now, the remaining shares must be sold at a higher price to get back to zero net loss/profit.

2. When the (some of the) shares are sold at a *higher* price than their original, the average price *decreases*.
    $$ P_2 > P_1  \implies \overline{P}_2 < \overline{P}_1 $$

    **Reason:** Some profit has been made. Now, the remaining shares must be sold at a lower price to get back to zero net profit/loss.

3. When the (some of the) shares are sold at the *same* price than their original, the average price *remains unchanged*.
    $$ P_2 = P_1  \implies \overline{P}_2 = \overline{P}_1 $$

