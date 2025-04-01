# API

The DMOJ supports a simple JSON API for accessing most data used by the backend. Access to the API makes use of API tokens.

## API tokens

The DMOJ supports API tokens for accessing the majority of the site as your native user. The admin portion of the site is left intentionally inaccessible with these tokens. You may generate an API token on your *Edit profile* page. To use, include the following header with every request where `<API Token>` is your API token:

```http
Authorization: Bearer <API Token>
```

### Error responses

The following error codes may be returned by the API token authentication layer. Note that the site itself may return other codes not listed here or identical codes with different error messages, so read the error messages carefully.

- `400 Invalid authorization header` - The **header** you provided is invalid. Make sure it matches the following regex: `Authorization: Bearer ([a-zA-Z0-9_-]{48})`
- `401 Invalid token` - The **token** you provided is invalid. Make sure it matches the one on your *Edit profile* page.
- `403 Admin inaccessible` - You are trying to access the inaccessible admin portion of the site.

## Rate limiting

**90 requests per minute**

If you exceed this limit, you will be captcha'd. Captchas are automatically removed after 3 days. However, note that if you are captcha'd again within this 3 day period, the 3 day counter will reset.

**Note**: This is only a feature on the [DMOJ site](https://dmoj.ca).

## Format

All responses are of the following structure:

```json
{
    "api_version": "2.0",
    "method": "<HTTP method that was used>",
    "fetched": "<time that the request was made in ISO format>",
    "data": "<rest of the data>",
    "error": "<any errors that were encountered>"
}
```

It is guaranteed that only one of `data` or `error` will be in the response.

### Error format

```json
{
    "error": {
        "code": "<HTTP status code>",
        "message": "<error message>"
    }
}
```

### Data format

The data format differs depending on the endpoint called. For endpoints that respond with a single object:

```json
{
    "data": {
        "object": "<object data>"
    }
}
```

For endpoints that respond with a list of objects:

```json
{
    "data": {
        "current_object_count": "<number of objects in the current page>",
        "objects_per_page": "<maximum number of objects that will ever appear on a single page>",
        "total_objects": "<total number of objects in the list>",
        "page_index": "<the current page's index, one indexed>",
        "total_pages": "<total number of pages>",
        "objects": [
            "<list of object data>"
        ]
    }
}
```

### Filtering

Most of the API endpoints support filtering via query parameters. There are two types of filtering that are supported, basic filtering and list filtering. Basic filtering allows filtering for a single value, while list filtering allows filtering for a group of values. Each endpoint describes the filtering that it supports, with the name in a codeblock being the query parameter name.

Example of basic filtering: `/api/v2/problems?partial=True` - This will only return problems with partial points enabled.

Example of list filtering: `/api/v2/problems?organization=1&organization=2&type=Implementation` - This will only return problems (private to organizations 1 OR 2) AND (problem type is Implementation).

## Endpoints

### `/api/v2/contests`

Example: [/api/v2/contests?tag=seasonal&tag=dmopc](https://dmoj.ca/api/v2/contests?tag=seasonal&tag=dmopc)

#### Basic filters

- `is_rated` - boolean

#### List filters

- `key` - contest key
- `tag` - tag name
- `organization` - organization id

#### Object response

```json
{
    "key": "<contest key>",
    "name": "<contest name>",
    "start_time": "<contest start time in ISO format>",
    "end_time": "<contest end time in ISO format>",
    "is_rated": "<whether the contest is rated>",
    "rate_all": "<whether the contest is rated on join>",
    "time_limit": "<contest time limit in seconds, or null if the contest is not windowed>",
    "tags": [
        "<list of tag name>"
    ]
}
```

### `/api/v2/contest/<contest key>`

Example: [/api/v2/contest/bts19](https://dmoj.ca/api/v2/contest/bts19)

#### Object response

```json
{
    "key": "<contest key>",
    "name": "<contest name>",
    "start_time": "<contest start time in ISO format>",
    "end_time": "<contest end time in ISO format>",
    "time_limit": "<contest time limit in seconds, or null if the contest is not windowed>",
    "is_rated": "<whether the contest is rated>",
    "rate_all": "<whether the contest is rated on join>",
    "has_rating": "<whether the contest has been rated>",
    "rating_floor": "<the minimum user rating required for the user to be rated, or null if there is no minimum>",
    "rating_ceiling": "<the maximum user rating for the user to be rated, or null if there is no maximum>",
    "performance_ceiling": "<the maximum user rating possible from this contest, or null if there is no maximum>",
    "hidden_scoreboard": "<whether the contest's scoreboard is hidden>",
    "scoreboard_visibility": "<whether the scoreboard is (V)isible, visible after (C)ontest, or visible after (P)articipation>",
    "is_organization_private": "<whether the contest is private to organizations>",
    "organizations": [
        "<list of organization id>"
    ],
    "is_private": "<whether the contest is private to specific users>",
    "tags": [
        "<list of tag name>"
    ],
    "format": {
        "name": "<the name of the contest format>",
        "config": "<the contest format JSON configuration>"
    },
    "problems": [
        {
            "points": "<the integer amount of points the problem is worth in contest>",
            "partial": "<whether it is possible to achieve partial points on the problem>",
            "is_pretested": "<whether the problem is pretested>",
            "max_submissions": "<the maximum number of submissions allowed, or null if there is no limit>",
            "label": "<the label for this problem>",
            "name": "<problem name>",
            "code": "<problem code>"
        }
    ],
    "rankings": [
        {
            "user": "<participant username>",
            "start_time": "<effective participation start time in ISO format>",
            "end_time": "<participation end time in ISO format>",
            "score": "<participant score>",
            "cumulative_time": "<participant cumulative time, dependent on the contest format>",
            "tiebreaker": "<participant tiebreaker value>",
            "old_rating": "<participant rating before the contest, or null if not rated>",
            "new_rating": "<participant rating after the contest, or null if not rated>",
            "is_disqualified": "<whether this participant is disqualified>",
            "solutions": [
                "<list of contest format-dependent dictionaries for individual problem scores>"
            ]
        }
    ]
}
```

### `/api/v2/participations`

Example: [/api/v2/participations?contest=dmopc19c6&virtual_participation_number=0&is_disqualified=True](https://dmoj.ca/api/v2/participations?contest=dmopc19c6&virtual_participation_number=0&is_disqualified=True)

#### Basic filters

- `contest` - contest key
- `user` - user username
- `is_disqualified` - boolean
- `virtual_participation_number` - non-negative integer

#### Object response

```json
{
    "user": "<participant username>",
    "contest": "<contest key>",
    "start_time": "<effective participation start time in ISO format>",
    "end_time": "<participation end time in ISO format>",
    "score": "<participant score>",
    "cumulative_time": "<participant cumulative time, dependent on the contest format>",
    "tiebreaker": "<participant tiebreaker value>",
    "is_disqualified": "<whether this participant is disqualified>",
    "virtual_participation_number": "<virtual participation number>"
}
```

### `/api/v2/problems`

Example: [/api/v2/problems?partial=True&type=Uncategorized](https://dmoj.ca/api/v2/problems?partial=True&type=Uncategorized)

#### Basic filters

- `partial` - boolean

#### List filters

- `code` - problem code
- `group` - problem group full name
- `type` - problem type full name
- `organization` - organization id

#### Additional filters

- `search` - similar to a list filter, except searches for the list of parameters in the problem's name, code, and description.

#### Object response

```json
{
    "code": "<problem code>",
    "name": "<problem name>",
    "types": [
        "<list of type full name>"
    ],
    "group": "<problem group full name>",
    "points": "<problem points>",
    "partial": "<whether partials are enabled for this problem>",
    "is_organization_private": "<whether the problem is private to organizations>",
    "is_public": "<whether the problem is publicly visible>"
}
```

### `/api/v2/problem/<problem code>`

Example: [/api/v2/problem/helloworld](https://dmoj.ca/api/v2/problem/helloworld)

#### Object response

```json
{
    "code": "<problem code>",
    "name": "<problem name>",
    "authors": [
        "<list of author username>"
    ],
    "types": [
        "<list of type full name>"
    ],
    "group": "<problem group full name>",
    "time_limit": "<problem time limit>",
    "memory_limit": "<problem memory limit>",
    "language_resource_limits": [
        {
            "language": "<language key>",
            "time_limit": "<language-specific time limit>",
            "memory_limit": "<language-specific memory limit>"
        }
    ],
    "points": "<problem points>",
    "partial": "<whether partials are enabled for this problem>",
    "short_circuit": "<whether short circuit is enabled for this problem>",
    "languages": [
        "<list of language key>"
    ],
    "is_organization_private": "<whether the problem is private to organizations>",
    "organizations": [
        "<list of organization id>"
    ],
    "is_public": "<whether the problem is publicly visible>"
}
```

#### Additional info

`is_public`: Whether the problem is publicly visible to the organizations listed. If `is_organization_private` is `false`, the problem is visible to all users.

### `/api/v2/users`

Example: [/api/v2/users?organization=8](https://dmoj.ca/api/v2/users?organization=8)

#### List filters

- `id` - user id
- `username` - user username
- `organization` - organization id

#### Object response

```json
{
    "id": "<user id>",
    "username": "<user username>",
    "points": "<user points>",
    "performance_points": "<user performance points>",
    "problem_count": "<number of problems the user has solved>",
    "rank": "<user display rank>",
    "rating": "<user rating>"
}
```

### `/api/v2/user/<user username>`

Example: [/api/v2/user/Xyene](https://dmoj.ca/api/v2/user/Xyene)

#### Object response

```json
{
    "id": "<user id>",
    "username": "<user username>",
    "points": "<user points>",
    "performance_points": "<user performance points>",
    "problem_count": "<number of problems the user has solved>",
    "solved_problems": [
        "<list of problem code>"
    ],
    "rank": "<user display rank>",
    "rating": "<user rating>",
    "organizations": [
        "<list of organization id>"
    ],
    "contests": [
        {
            "key": "<contest key>",
            "score": "<user score>",
            "cumulative_time": "<user cumulative time, dependent on the contest format>",
            "rating": "<user rating after this contest, or null if not rated>",
            "raw_rating": "<user raw rating after this contest, or null if not rated>",
            "performance": "<user performance, or null if not rated>"
        }
    ]
}
```

### `/api/v2/submissions`

Example: [/api/v2/submissions?user=Ninjaclasher](https://dmoj.ca/api/v2/submissions?user=Ninjaclasher)

#### Basic filters

- `user` - user username
- `problem` - problem code

#### List filters

- `id` - submission id
- `language` - language key
- `result` - string

#### Object response

```json
{
    "id": "<submission id>",
    "problem": "<problem code>",
    "user": "<user username>",
    "date": "<submission date in ISO format>",
    "language": "<language key>",
    "time": "<submission time usage>",
    "memory": "<submission memory usage>",
    "points": "<submission points awarded>",
    "result": "<submission result>"
}
```

### `/api/v2/submission/<submission id>`

Example: [/api/v2/submission/1000000](https://dmoj.ca/api/v2/submission/1000000)

#### Object response

```json
{
    "id": "<submission id>",
    "problem": "<problem code>",
    "user": "<user username>",
    "date": "<submission date in ISO format>",
    "time": "<submission time usage>",
    "memory": "<submission memory usage>",
    "points": "<submission points awarded>",
    "language": "<language key>",
    "status": "<submission status>",
    "result": "<submission result>",
    "case_points": "<submission case points>",
    "case_total": "<submission case total>",
    "cases": [
        "<list of case or batch data>"
    ]
}
```

#### Additional info

`case or batch data`: Each object will be one of the following, depending on whether the current case is a batch or a single test case:

#### Case data

```json
{
    "type": "case",
    "case_id": "<case id>",
    "status": "<case status>",
    "time": "<case time usage>",
    "memory": "<case memory usage>",
    "points": "<case points awarded>",
    "total": "<case total points>"
}
```

#### Batch data

```json
{
    "type": "batch",
    "batch_id": "<batch id>",
    "cases": [
        "<list of case data>"
    ],
    "points": "<batch points awarded>",
    "total": "<batch total points>"
}
```

### `/api/v2/organizations`

Example: [/api/v2/organizations?is_open=False](https://dmoj.ca/api/v2/organizations?is_open=False)

#### Basic filters

- `is_open` - boolean

#### List filters
- `id` - organization id

#### Object response

```json
{
    "id": "<organization id>",
    "slug": "<organization slug>",
    "short_name": "<organization name>",
    "is_open": "<whether anyone can join the organization>",
    "member_count": "<number of users in the organization>"
}
```

### `/api/v2/languages`

Example: [/api/v2/languages?common_name=Python](https://dmoj.ca/api/v2/languages?common_name=Python)

#### Basic filters

- `common_name` - language common name

#### List filters
- `id` - language id
- `key` - language key

#### Object response

```json
{
    "id": "<language id>",
    "key": "<language key>",
    "short_name": "<language short name>",
    "common_name": "<language common name>",
    "ace_mode_name": "<Ace mode name>",
    "pygments_name": "<Pygments name>",
    "code_template": "<default code template>"
}
```

### `/api/v2/judges`

Example: [/api/v2/judges](https://dmoj.ca/api/v2/judges)

#### Object response

```json
{
    "name": "<judge name>",
    "start_time": "<judge start time in ISO format>",
    "ping": "<judge ping in milliseconds>",
    "load": "<judge load>",
    "languages": [
        "<list of language key>"
    ]
}
```
