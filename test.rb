describe 'database' do
  before(:each)do
    'rm -rf test.db'
  end

  def run_script(commands)
    raw_output = nil
    IO.popen("./db test.db", "r+") do |pipe|
      commands.each do |command|
        begin
          pipe.puts command
        rescue Error::EPIPE
          break
        end
      end

      pipe.close_write

      # Read entire output
      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end

  it 'prints error message if strings are too long' do
    long_username = "a" * 33
    long_email = "b" * 256
    script = [
      "insert 1 #{long_username} #{long_email}",
      "select",
      ".exit",
    ]
    result = run_script(script)
    expect(result).to match_array([
      "db > String is too long.",
      "db > Executed.",
      "db > ",
    ])
  end

  it 'prints error message if id is negative' do
    script = [
      "insert -1 user user@email.com",
      "select",
      ".exit",
    ]
    result = run_script(script)
    expect(result).to match_array([
      "db > ID must be positive.",
      "db > Executed.",
      "db > ",
    ])
  end

  it 'prints constants' do
    script = [
      ".constants",
      ".exit",
    ]
    result = run_script(script)
    expect(result).to match_array([
      "db > Constants:",
      "ROW_SIZE: 293",
      "COMMON_NODE_HEAD_SIZE: 6",
      "LEAF_NODE_HEADER_SIZE: 14",
      "LEAF_NODE_CELL_SIZE: 297",
      "LEAF_NODE_SPACE_FOR_CELLS: 4082",
      "LEAF_NODE_MAX_CELLS: 13",
      "db > ",
    ])
  end

  it 'prints an error message if there is a duplicate id' do
    script  = [
      "insert 1 user1 person1@email.com",
      "insert 1 user2 person2@email.com",
      ".exit",
    ]    
    result = run_script(script)
    expect(result).to match_array([
      "db > Executed.",
      "db > Error: Duplicate key.",
      "db > ",
    ])
  end

it 'allows printing out the structure of a 3-leaf-node btree' do
    script = (1..14).map do |i|
      "insert #{i} user#{i} person#{i}@example.com"
    end
    script << ".btree"
    script << "insert 15 user15 person15@example.com"
    script << ".exit"
    result = run_script(script)

    expect(result[14...(result.length)]).to match_array([
      "db > Tree:",
      "- internal (size 1)",
      "  - leaf (size 7)",
      "    - 1",
      "    - 2",
      "    - 3",
      "    - 4",
      "    - 5",
      "    - 6",
      "    - 7",
      "  - key 7",
      "  - leaf (size 7)",
      "    - 8",
      "    - 9",
      "    - 10",
      "    - 11",
      "    - 12",
      "    - 13",
      "    - 14",
      "db > Executed.",
      "db > ",
    ])
  end

  it 'prints all rows in a multi-level tree' do
    script = []
    (1..15).each do |i|
      script << "insert #{i} user#{i} person#{i}@email.com"
    end
    script << "select"
    script << ".exit"
    result = run_script(script)

    expect(result[15...result.length]).to match_array([
      "db > (1, user1, person1@email.com)",
      "(2, user2, person2@email.com)",
      "(3, user3, person3@email.com)",
      "(4, user4, person4@email.com)",
      "(5, user5, person5@email.com)",
      "(6, user6, person6@email.com)",
      "(7, user7, person7@email.com)",
      "(8, user8, person8@email.com)",
      "(9, user9, person9@email.com)",
      "(10, user10, person10@email.com)",
      "(11, user11, person11@email.com)",
      "(12, user12, person12@email.com)",
      "(13, user13, person13@email.com)",
      "(14, user14, person14@email.com)",
      "(15, user15, person15@email.com)",
      "Executed.",
      "db > ",
    ])    
  end

end